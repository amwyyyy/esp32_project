/* LVGL Example project
 *
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "event.h"
#include "storage.h"
#include "pwm.h"
#include "gui.h"
#include "wifi.h"
#include "sntp_time.h"
#include "weather.h"

#define TAG "main"

xQueueHandle basic_evt_queue;

static void event_handle(void *pvParameter);

/**
 * @brief 开机自动连接wifi
 *
 */
void connect_wifi() {
    char get_ssid[32];
    char get_pwd[64];

    if (get_wifi_info(get_ssid, get_pwd) == ESP_OK) {
        ESP_LOGI(TAG, "ssid信息有效 进行开机自动连接wifi ssid: %s pwd: %s", get_ssid, get_pwd);
        wifi_init_sta(get_ssid, get_pwd);
    }
}

static void event_handle(void *pvParameter) {
    uint32_t event_flag;

    while (1) {
        if (xQueueReceive(basic_evt_queue, &event_flag, portMAX_DELAY)) {
            if (event_flag == EVENT_WIFI_STA_CONNECTED) {
                printf("连接 wifi 成功!!!\n");
                sntp_time_init();
            } else if (event_flag == EVENT_SNTP_INIT) {
                printf("同步 SNTP 时间成功\n");
                gui_init();
            }
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    basic_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    storage_init();

    pwm_init();

    set_wifi_info("xiongda", "15999554794");

    connect_wifi();

    xTaskCreate(event_handle, "event", 1024 * 2, NULL, 1, NULL);

    ESP_LOGI(TAG, "[APP] Free internal memory: %d kb", esp_get_free_internal_heap_size() / 1024);
    ESP_LOGI(TAG, "[APP] Free all memory: %d kb", esp_get_free_heap_size() / 1024);
    ESP_LOGI(TAG, "[APP] Min free memory: %d kb", esp_get_minimum_free_heap_size() /1024);
    ESP_LOGI(TAG, "[APP] Idf version: %s", esp_get_idf_version());
}