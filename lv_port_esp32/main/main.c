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
static void update_data_task(void *pvParameter);

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

/**
 * @brief 定时更新数据
 * 
 * @param pvParameter 
 */
static void update_data_task(void *pvParameter) {
    while (1) {
        vTaskDelay(1000 * 60 * 60 / portTICK_PERIOD_MS);
        weather_t wea = weather_init();
        set_weather_info(wea);
    }
}

static void event_handle(void *pvParameter) {
    uint32_t event_flag;

    while (1) {
        if (xQueueReceive(basic_evt_queue, &event_flag, portMAX_DELAY)) {
            if (event_flag == EVENT_WIFI_STA_CONNECTED) {
                set_loading_text("Sntp init...");
                sntp_time_init();

                set_loading_text("Weather init...");
                weather_t wea = weather_init();
                set_weather_info(wea);
            } else if (event_flag == EVENT_SNTP_INIT) {
                set_loading_text("Success!");
                vTaskDelay(500 / portTICK_PERIOD_MS);

                // 显示天气时间界面
                display(DISP_CLOCK);

                // 启动更新数据任务
                xTaskCreate(update_data_task, "update", 1024 * 8, NULL, 1, NULL);
            } else if (event_flag == EVENT_WIFI_STA_START) {
                set_loading_text("Wifi init...");
            } else if (event_flag == EVENT_WIFI_STA_DISCONNECTED) {
                set_loading_text("Wifi error...");
                vTaskDelay(1000 / portTICK_PERIOD_MS);

                ESP_LOGI(TAG, "Wifi connect error restart.");
                esp_restart();
            } else if (event_flag == EVENT_SINT_FAILURE) {
                set_loading_text("Sntp error...");
                vTaskDelay(1000 / portTICK_PERIOD_MS);

                ESP_LOGI(TAG, "Sntp sync error restart.");
                esp_restart();
            }
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    basic_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    storage_init();

    pwm_init();

    set_bl_pwm(1);

    gui_init();

    // set_wifi_info("xiongda", "15999554794");

    connect_wifi();

    xTaskCreate(event_handle, "event", 1024 * 8, NULL, 1, NULL);

    ESP_LOGI(TAG, "[APP] Free internal memory: %d kb", esp_get_free_internal_heap_size() / 1024);
    ESP_LOGI(TAG, "[APP] Free all memory: %d kb", esp_get_free_heap_size() / 1024);
    ESP_LOGI(TAG, "[APP] Min free memory: %d kb", esp_get_minimum_free_heap_size() /1024);
    ESP_LOGI(TAG, "[APP] Idf version: %s", esp_get_idf_version());
}