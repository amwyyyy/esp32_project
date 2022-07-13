/*
 * SPDX-FileCopyrightText: 2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

static const char *TAG = "ADC SINGLE";

#define HUMAN_GPIO 10

void adc1task(void* arg)
{
     adc1_config_width(ADC_WIDTH_BIT_DEFAULT); // 设定捕获宽度
     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // 设定捕获通道
     while(1){
         printf("The adc1 value: %d\n", adc1_get_raw(ADC1_CHANNEL_0)); // 读取在单个通道上的ADC读数
         vTaskDelay(1000 / portTICK_PERIOD_MS);
     }
}

void read_hc_sr501() {
    gpio_reset_pin(HUMAN_GPIO);  // 重置GPIO
    gpio_set_direction(HUMAN_GPIO, GPIO_MODE_INPUT); // 将GPIO设置为输入模式

    gpio_reset_pin(19);
    gpio_set_direction(19, GPIO_MODE_OUTPUT);

    while (1) {
        // 根据GPIO的高低电平判断是否感应到人体活动
        int status = gpio_get_level(HUMAN_GPIO);
        ESP_LOGI(TAG, "是否人体活动 %s!", status == 0 ? "无" : "有");
        gpio_set_level(19, status);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(adc1task, "test_task", 8192, NULL, 5, NULL);
    xTaskCreate(read_hc_sr501, "read_hc_sr501", 8192, NULL, 5, NULL);
}