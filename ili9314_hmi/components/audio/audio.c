// Copyright 2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_heap_caps.h"
#include "esp_spiffs.h"
#include "driver/i2s.h"
#include "audio.h"
#include "esp_log.h"
#include "es8311.h"
#include "mp3dec.h"
#include "es8311.h"

static const char *TAG = "AUDIO";

#define SAMPLE_RATE (44100)
#define I2S_NUM (0)
#define WAVE_FREQ_HZ (100)
#define PI (3.14159265)

#define SAMPLE_PER_CYCLE (SAMPLE_RATE / WAVE_FREQ_HZ)

xQueueHandle play_queue = NULL;

/*!< aduio music list from spiffs*/
const char audio_list[AUDIO_MAX_PLAY_LIST][64] = {
    "/spiffs/apple-tosk.mp3",
    "/spiffs/Beautiful.mp3",
    "/spiffs/distance.mp3",
    "/spiffs/longest_movie.mp3",
    "/spiffs/To_meet_44k.mp3",
};

enum
{
    AUDIO_STOP = 0,
    AUDIO_PLAY,
    AUDIO_NEXT,
    AUDIO_LAST
};

int play_flag = AUDIO_STOP;
int audio_play_index = 0;

void aplay_mp3(const char *path)
{
    ESP_LOGI(TAG, "start to decode %s", path);
    HMP3Decoder hMP3Decoder;
    MP3FrameInfo mp3FrameInfo;
    unsigned char *readBuf = malloc(MAINBUF_SIZE);

    if (readBuf == NULL)
    {
        ESP_LOGE(TAG, "readBuf malloc failed");
        return;
    }

    short *output = malloc(1153 * 4);

    if (output == NULL)
    {
        free(readBuf);
        ESP_LOGE(TAG, "outBuf malloc failed");
    }

    hMP3Decoder = MP3InitDecoder();

    if (hMP3Decoder == 0)
    {
        free(readBuf);
        free(output);
        ESP_LOGE(TAG, "memory is not enough..");
    }

    int samplerate = 0;
    i2s_zero_dma_buffer(0);
    FILE *mp3File = fopen(path, "rb");

    if (mp3File == NULL)
    {
        MP3FreeDecoder(hMP3Decoder);
        free(readBuf);
        free(output);
        ESP_LOGE(TAG, "open file failed");
    }

    char tag[10];
    int tag_len = 0;
    int read_bytes = fread(tag, 1, 10, mp3File);

    if (read_bytes == 10)
    {
        if (memcmp(tag, "ID3", 3) == 0)
        {
            tag_len = ((tag[6] & 0x7F) << 21) | ((tag[7] & 0x7F) << 14) | ((tag[8] & 0x7F) << 7) | (tag[9] & 0x7F);
            // ESP_LOGI(TAG,"tag_len: %d %x %x %x %x", tag_len,tag[6],tag[7],tag[8],tag[9]);
            fseek(mp3File, tag_len - 10, SEEK_SET);
        }
        else
        {
            fseek(mp3File, 0, SEEK_SET);
        }
    }

    int bytesLeft = 0;
    unsigned char *readPtr = readBuf;
    play_flag = AUDIO_PLAY;

    while (1)
    {
        switch (play_flag)
        {
        case AUDIO_STOP:
        {
            while (!play_flag)
            {
                i2s_zero_dma_buffer(0);
                vTaskDelay(100 / portTICK_RATE_MS);
                goto stop;
            }
            break;
        }
        break;

        case AUDIO_PLAY:
        {
        }
        break;

        case AUDIO_NEXT:
        {
            if (audio_play_index < AUDIO_MAX_PLAY_LIST - 1)
            {
                audio_play_index++;
            }
            else
            {
                audio_play_index = 0;
            }

            goto stop;
        }
        break;

        case AUDIO_LAST:
        {
            if (audio_play_index > 0)
            {
                audio_play_index--;
            }
            else
            {
                audio_play_index = AUDIO_MAX_PLAY_LIST - 1;
            }

            goto stop;
        }
        break;
        }

        if (bytesLeft < MAINBUF_SIZE)
        {
            memmove(readBuf, readPtr, bytesLeft);
            int br = fread(readBuf + bytesLeft, 1, MAINBUF_SIZE - bytesLeft, mp3File);

            if ((br == 0) && (bytesLeft == 0))
            {
                break;
            }

            bytesLeft = bytesLeft + br;
            readPtr = readBuf;
        }

        int offset = MP3FindSyncWord(readPtr, bytesLeft);

        if (offset < 0)
        {
            ESP_LOGE(TAG, "MP3FindSyncWord not find");
            bytesLeft = 0;
            continue;
        }
        else
        {
            readPtr += offset;   /*!< data start point */
            bytesLeft -= offset; /*!< in buffer */
            int errs = MP3Decode(hMP3Decoder, &readPtr, &bytesLeft, output, 0);

            if (errs != 0)
            {
                ESP_LOGE(TAG, "MP3Decode failed ,code is %d ", errs);
                break;
            }

            MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);

            if (samplerate != mp3FrameInfo.samprate)
            {
                samplerate = mp3FrameInfo.samprate;
                i2s_set_clk(0, samplerate, 16, mp3FrameInfo.nChans);
                ESP_LOGI(TAG, "mp3file info---bitrate=%d,layer=%d,nChans=%d,samprate=%d,outputSamps=%d", mp3FrameInfo.bitrate, mp3FrameInfo.layer, mp3FrameInfo.nChans, mp3FrameInfo.samprate, mp3FrameInfo.outputSamps);
            }

            size_t bytes_write = 0;
            i2s_write(0, (const char *)output, mp3FrameInfo.outputSamps * 2, &bytes_write, 100 / portTICK_RATE_MS);
            // rmt_write_items(0,(const char*)output,mp3FrameInfo.outputSamps*2, 1000 / portTICK_RATE_MS);
        }
    }

stop:
    i2s_zero_dma_buffer(0);
    MP3FreeDecoder(hMP3Decoder);
    free(readBuf);
    free(output);
    fclose(mp3File);

    ESP_LOGI(TAG, "end mp3 decode ..");
}

static void audio_task(void *arg)
{
    /*!<  for 36Khz sample rates, we create 100Hz sine wave, every cycle need 36000/100 = 360 samples (4-bytes or 8-bytes each sample) */
    /*!<  depend on bits_per_sample */
    /*!<  using 6 buffers, we need 60-samples per buffer */
    /*!<  if 2-channels, 16-bit each channel, total buffer is 360*4 = 1440 bytes */
    /*!<  if 2-channels, 24/32-bit each channel, total buffer is 360*8 = 2880 bytes */
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX, /*!<  Only TX */
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, /*!< 1-channels */
        .communication_format = I2S_COMM_FORMAT_I2S,
        .dma_buf_count = 6,
        .dma_buf_len = 256,
        .use_apll = true,
        .tx_desc_auto_clear = true, /*!< I2S auto clear tx descriptor if there is underflow condition (helps in avoiding noise in case of data unavailability) */
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCLK,
        .ws_io_num = I2S_LCLK,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_DSIN /*!< Not used */
    };

    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM, &pin_config);

    unsigned int mp3_index = 0;
    while (1)
    {
        if (xQueueReceive(play_queue, &mp3_index, portTICK_RATE_MS))
        {
            printf("play_queue: mp3_index = %u\n", mp3_index);

            if (mp3_index > (AUDIO_MAX_PLAY_LIST - 1))
                printf("mp3_index: error\n");
            else
            {
                aplay_mp3(audio_list[mp3_index]);
                vTaskDelay(1000 / portTICK_RATE_MS);
            }
        }
    }
}

int audio_init(uint32_t vol)
{
    es8311_init(SAMPLE_RATE);
    
    if (vol > 100)
        vol = 100;

    ESP_LOGI(TAG, "设置音量(%d)", vol);
    es8311_set_voice_volume(vol);
    xTaskCreate(audio_task, "audio_task", 4096, NULL, 5, NULL);
    return 0;
}

void audio_setting_vol(uint32_t vol)
{
    if (vol > 100)
        vol = 100;

    es8311_set_voice_volume(vol);
}