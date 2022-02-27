#include <stdio.h>
#include "sdkconfig.h"

#include "esp_heap_caps.h"
#include "esp_spi_flash.h"
#include "esp_system.h"

#include "lvgl_port.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "audio.h"
#include "nvs_flash.h"
#include "esp_spiffs.h"
#include "i2c_bus2.h"
#include "music.h"
#include "gpio.h"
#include "color.h"
#include "main.h"
#include "storage.h"
#include "wifi.h"
#include "wifi_scan.h"
#include "wifi_scan_page.h"
#include "main_page.h"
#include "lv_symbol_extra_def.h"
#include "weather_page.h"
#include "lvgl_port.h"
#include "lv_port_fs.h"
#include "music_page.h"
#include "task_mag.h"
#include "setting_page.h"

#define TAG "TASK_MAG"

extern xQueueHandle main_page_queue;
extern xQueueHandle wifi_scan_page_queue;
extern xQueueHandle weather_page_queue;
extern xQueueHandle music_page_queue;
extern xQueueHandle setting_page_queue;

static TaskHandle_t task_handle = NULL;
PAGE_TASK now_task = NUMM_TASK_NUM;

static xQueueHandle now_runing_page_queue = NULL;

/**
 * @brief 启动页面任务
 *
 * @param num
 */
void cql_page_init(PAGE_TASK num)
{
    if (task_handle != NULL)
    {
        ESP_LOGE(TAG, "已有页面在运行 启动页面失败");
        return;
    }

    ESP_LOGE(TAG, "启动页面(%d)", num);
    switch (num)
    {
    case MAIN_PAGE_TASK_NUM:
        if (main_page_queue == NULL)
            main_page_queue = xQueueCreate(6, sizeof(uint32_t));

        now_runing_page_queue = main_page_queue;
        xTaskCreate(main_page_task, "main_page_task", 1024 * 4, NULL, 5, NULL);
        break;
    case WIFI_SCAN_TASK_NUM:
        now_task = num;
        if (wifi_scan_page_queue == NULL)
            wifi_scan_page_queue = xQueueCreate(6, sizeof(uint32_t));

        now_runing_page_queue = wifi_scan_page_queue;
        xTaskCreate(wifi_scan_page_task, "wifi_scan_page_task", 1024 * 4, NULL, 5, &task_handle);
        break;
    case WEATHER_PAGE_TASK_NUM:
        now_task = num;
        if (weather_page_queue == NULL)
            weather_page_queue = xQueueCreate(6, sizeof(uint32_t));

        now_runing_page_queue = weather_page_queue;
        xTaskCreate(weather_page_task, "weather_page_task", 1024 * 4, NULL, 5, &task_handle);
        break;
    case MUSIC_PAGE_TASK_NUM:
        now_task = num;
        if (music_page_queue == NULL)
            music_page_queue = xQueueCreate(6, sizeof(uint32_t));

        now_runing_page_queue = music_page_queue;
        xTaskCreate(music_page_task, "music_page_task", 1024 * 4, NULL, 5, &task_handle);
        break;
    case SETTING_PAGE_TASK_NUM:
        now_task = num;
        if (setting_page_queue == NULL)
            setting_page_queue = xQueueCreate(6, sizeof(uint32_t));

        now_runing_page_queue = setting_page_queue;
        xTaskCreate(setting_page_task, "setting_page_task", 1024 * 4, NULL, 5, &task_handle);
        break;
    default:
        break;
    }
}

/**
 * @brief  发送键盘键值
 *
 * @param key 键值
 */
void keyboard_sned_key(uint32_t key)
{
    uint32_t page_option_data;
    switch (key)
    {
    case KEYBOARD_1: //左键

        page_option_data = PAGE_LEFT_KEY;
        xQueueSend(now_runing_page_queue, &page_option_data, NULL);
        break;
    case KEYBOARD_3: //右键

        page_option_data = PAGE_RIGHT_KEY;
        xQueueSend(now_runing_page_queue, &page_option_data, NULL);
        break;
    default:
        break;
    }
}

/**
 * @brief 删除页面任务
 *
 * @param num
 */
void cql_page_dele(PAGE_TASK num)
{
    TaskHandle_t task;

    if (now_task != num)
    {
        ESP_LOGE(TAG, "删除页面失败 该为无效页面");
        return;
    }

    ESP_LOGE(TAG, "删除页面(%d)", num);
    task = task_handle;
    task_handle = NULL;
    vTaskDelete(task);
}
