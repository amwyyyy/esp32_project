/* LEDC (LED Controller) fade example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

/*
 * About this example
 *
 * 1. Start with initializing LEDC module:
 *    a. Set the timer of LEDC first, this determines the frequency
 *       and resolution of PWM.
 *    b. Then set the LEDC channel you want to use,
 *       and bind with one of the timers.
 *
 * 2. You need first to install a default fade function,
 *    then you can use fade APIs.
 *
 * 3. You can also set a target duty directly without fading.
 *
 * 4. This example uses GPIO18/19/4/5 as LEDC output,
 *    and it will change the duty repeatedly.
 *
 * 5. GPIO18/19 are from high speed channel group.
 *    GPIO4/5 are from low speed channel group.
 *
 */


#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_LS_CH0_GPIO       (3)
#define LEDC_LS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_LS_CH1_GPIO       (4)
#define LEDC_LS_CH1_CHANNEL    LEDC_CHANNEL_1
#define LEDC_LS_CH2_GPIO       (5)
#define LEDC_LS_CH2_CHANNEL    LEDC_CHANNEL_2

#define LEDC_TEST_CH_NUM       (3)
#define LEDC_TEST_DUTY         (2000)       // PWM 占空比，可以设置亮度
#define LEDC_TEST_FADE_TIME    (3000)       // 逐渐变化的时间

void app_main(void)
{
    int ch;

    gpio_reset_pin(18); 
    gpio_set_direction(18, GPIO_MODE_OUTPUT);
    gpio_set_level(18, 0);

    gpio_reset_pin(19);
    gpio_set_direction(19, GPIO_MODE_OUTPUT);
    gpio_set_level(19, 0);

    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT,  // resolution of PWM duty PWM占空比分辨率
        .freq_hz = 5000,                       // frequency of PWM signal PWM信号频率
        .speed_mode = LEDC_LS_MODE,            // timer mode
        .timer_num = LEDC_LS_TIMER,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock  自动选择timer时钟
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    /*
     * Prepare individual configuration
     * for each channel of LED Controller
     * by selecting:
     * - controller's channel number
     * - output duty cycle, set initially to 0
     * - GPIO number where LED is connected to
     * - speed mode, either high or low
     * - timer servicing selected channel
     *   Note: if different channels use one timer,
     *         then frequency and bit_num of these channels
     *         will be the same
     */
    ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM] = {
        {
            .channel    = LEDC_LS_CH0_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH0_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
        },
        {
            .channel    = LEDC_LS_CH1_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH1_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
        },
        {
            .channel    = LEDC_LS_CH2_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH2_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
        },
    };

    // Set LED Controller with previously prepared configuration
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }

    // Initialize fade service.
    ledc_fade_func_install(0);

    while (1) {
        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
            ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
                    ledc_channel[ch].channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
            ledc_fade_start(ledc_channel[ch].speed_mode,
                    ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
            vTaskDelay(LEDC_TEST_FADE_TIME / 3 / portTICK_PERIOD_MS);
        }
        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
            ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
                    ledc_channel[ch].channel, 0, LEDC_TEST_FADE_TIME);
            ledc_fade_start(ledc_channel[ch].speed_mode,
                    ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
            vTaskDelay(LEDC_TEST_FADE_TIME / 3 / portTICK_PERIOD_MS);
        }
    }

    // while (1) {
        // printf("1. LEDC fade up to duty = %d\n", LEDC_TEST_DUTY);
        // for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        //     ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
        //             ledc_channel[ch].channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
        //     ledc_fade_start(ledc_channel[ch].speed_mode,
        //             ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
        // }
        // vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

        // printf("2. LEDC fade down to duty = 0\n");
        // for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        //     ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
        //             ledc_channel[ch].channel, 0, LEDC_TEST_FADE_TIME);
        //     ledc_fade_start(ledc_channel[ch].speed_mode,
        //             ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
        // }
        // vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

        // printf("3. LEDC set duty = %d without fade\n", LEDC_TEST_DUTY);
        // for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        //     ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_TEST_DUTY);
        //     ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
        // }
        // vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

        // printf("4. LEDC set duty = 0 without fade\n");
        // for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        //     ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0);
        //     ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
        // }
        // vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);
    // }

    // uint32_t value = 3;

    // uint32_t bl = (0xFF - 250) + (value * 25);
    // printf("3. LEDC set duty = %ld without fade\n", bl);
    // for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++)
    // {
    //     ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, bl);
    //     ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
    // }
}
