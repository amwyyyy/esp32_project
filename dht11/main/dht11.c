/*
 * MIT License
 * 
 * Copyright (c) 2018 Michele Biondi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "esp_timer.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dht11.h"

#if CONFIG_IDF_TARGET_ESP32
    #include "esp32/rom/ets_sys.h"
#elif CONFIG_IDF_TARGET_ESP32S2
    #include "esp32s2/rom/ets_sys.h"
#elif CONFIG_IDF_TARGET_ESP32C3
    #include "esp32c3/rom/ets_sys.h"
#elif CONFIG_IDF_TARGET_ESP32S3
    #include "esp32s3/rom/ets_sys.h"
#endif

static gpio_num_t dht_gpio;
static int64_t last_read_time = -2000000;
static struct dht11_reading last_read;

static int _waitOrTimeout(uint16_t microSeconds, int level) {
    int micros_ticks = 0;
    while(gpio_get_level(dht_gpio) == level) { 
        if(micros_ticks++ > microSeconds) {
            return DHT11_TIMEOUT_ERROR;
        }
        ets_delay_us(1);
    }
    return micros_ticks;
}

static int _checkCRC(uint8_t data[]) {
    if(data[4] == ((data[0] + data[1] + data[2] + data[3])&0xFF))
        return DHT11_OK;
    else
        return DHT11_CRC_ERROR;
}

/**
 * 发送开始信号
 */
static void _sendStartSignal() {
    gpio_set_direction(dht_gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(dht_gpio, 0);
    ets_delay_us(20 * 1000);
    gpio_set_level(dht_gpio, 1);
    ets_delay_us(40);
    gpio_set_direction(dht_gpio, GPIO_MODE_INPUT);
}

static int _checkResponse() {
    /* Wait for next step ~80us*/
    if(_waitOrTimeout(80, 0) == DHT11_TIMEOUT_ERROR) {
        return DHT11_TIMEOUT_ERROR;
    }

    /* Wait for next step ~80us*/
    if(_waitOrTimeout(80, 1) == DHT11_TIMEOUT_ERROR) {
        return DHT11_TIMEOUT_ERROR;
    }

    return DHT11_OK;
}

static struct dht11_reading _timeoutError() {
    struct dht11_reading timeoutError = {DHT11_TIMEOUT_ERROR, -1, -1};
    return timeoutError;
}

static struct dht11_reading _crcError() {
    struct dht11_reading crcError = {DHT11_CRC_ERROR, -1, -1};
    return crcError;
}

void DHT11_init(gpio_num_t gpio_num) {
    /* Wait 1 seconds to make the device pass its initial unstable status */
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    dht_gpio = gpio_num;
}

/**
 * 一次完整的数据传输为40bit, 高位先出: 8bit湿度整数数据 + 8bit湿度小数数据 + 8bi温度整数数据 + 8bit温度小数数据 + 8bit校验和
 * 
 * "0" 的格式为：50 微秒低电平，26~28 微秒高电平。"1" 的格式为：50 微秒低电平，加70微秒高电平
 */
struct dht11_reading DHT11_read() {
    /* Tried to sense too soon since last read (dht11 needs ~2 seconds to make a new read) */
    if(esp_timer_get_time() - 2000000 < last_read_time) {
        return last_read;
    }

    last_read_time = esp_timer_get_time();

    uint8_t data[5] = {0,0,0,0,0};

    _sendStartSignal();

    if(_checkResponse() == DHT11_TIMEOUT_ERROR) {
        return last_read = _timeoutError();
    }
    
    /* Read response */
    for(int i = 0; i < 40; i++) {
        /* Initial data */
        if(_waitOrTimeout(50, 0) == DHT11_TIMEOUT_ERROR) {
            return last_read = _timeoutError();
        }
                
        if(_waitOrTimeout(70, 1) > 28) {
            /* Bit received was a 1 */
            data[i/8] |= (1 << (7-(i%8)));
        }
    }

    if(_checkCRC(data) != DHT11_CRC_ERROR) {
        last_read.status = DHT11_OK;
        last_read.temperature = data[2] + (double)data[3] / (double)10;
        last_read.humidity = data[0] + (double)data[1] / (double)10;
        return last_read;
    } else {
        return last_read = _crcError();
    }
}