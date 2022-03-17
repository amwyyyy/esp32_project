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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "storage.h"
#include "pwm.h"
#include "gui.h"
#include "wifi.h"

// #include "myfont_3500hz_18.c"
// LV_FONT_DECLARE(myfont_3500hz_18);

#define TAG "tv"

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

void app_main() {
    storage_init();

    pwm_init();

    gui_init();

    set_wifi_info("xiongda", "15999554794");

    connect_wifi();

    ESP_LOGI(TAG, "[APP] Free internal memory: %d kb", esp_get_free_internal_heap_size() / 1024);
    ESP_LOGI(TAG, "[APP] Free all memory: %d kb", esp_get_free_heap_size() / 1024);
    ESP_LOGI(TAG, "[APP] Min free memory: %d kb", esp_get_minimum_free_heap_size() /1024);
    ESP_LOGI(TAG, "[APP] Idf version: %s", esp_get_idf_version());
}