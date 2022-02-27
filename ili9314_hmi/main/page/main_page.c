/**
 * @file hello_world_main.c
 * @brief Hello World Example
 * @version 0.1
 * @date 2021-03-07
 *
 * @copyright Copyright 2021 Espressif Systems (Shanghai) Co. Ltd.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

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
#include "pwm.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "cJSON.h"
#include "strings.h"
#include "http_time.h"

#define TAG "MAIN_PAGE"

extern xQueueHandle play_queue;

extern xQueueHandle wifi_scan_page_queue;
extern xQueueHandle weather_page_queue;
extern xQueueHandle music_page_queue;
extern xQueueHandle setting_page_queue;

xQueueHandle main_page_queue = NULL;

extern lv_group_t *group;
extern lv_group_t *Get_group(void);
static lv_obj_t *get_list_fun_obj(void);
static lv_obj_t *list_fun2;
static lv_obj_t *btn_wifi2 = NULL;
uint32_t wiif_connect_state = WIFI_CONNECT_NULL;

/**
 * @brief 功能列表事件回调
 *
 * @param obj 列表对象
 * @param event 事件
 */
static void list_event_handler(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;

    if (event == LV_EVENT_CLICKED)
    {
        printf("Clicked: %s\n", lv_list_get_btn_text(obj));
        uint32_t index = lv_list_get_btn_index(get_list_fun_obj(), obj);
        printf("lv_list_get_btn_index = %d\r\n", index);
        switch (index)
        {

        case 0:
            cql_page_init(WEATHER_PAGE_TASK_NUM);

            page_option_data = PAGE_HIDDEN;
            xQueueSend(main_page_queue, &page_option_data, NULL);
            page_option_data = PAGE_SHOW;
            xQueueSend(weather_page_queue, &page_option_data, NULL);
            break;
        case 1:
            cql_page_init(MUSIC_PAGE_TASK_NUM);

            page_option_data = PAGE_HIDDEN;
            xQueueSend(main_page_queue, &page_option_data, NULL);
            page_option_data = PAGE_SHOW;
            xQueueSend(music_page_queue, &page_option_data, NULL);
            break;
        case 2:
            cql_page_init(WIFI_SCAN_TASK_NUM);

            page_option_data = PAGE_HIDDEN;
            xQueueSend(main_page_queue, &page_option_data, NULL);
            page_option_data = PAGE_SHOW;
            xQueueSend(wifi_scan_page_queue, &page_option_data, NULL);
            break;
        case 3:
            cql_page_init(SETTING_PAGE_TASK_NUM);

            page_option_data = PAGE_HIDDEN;
            xQueueSend(main_page_queue, &page_option_data, NULL);
            page_option_data = PAGE_SHOW;
            xQueueSend(setting_page_queue, &page_option_data, NULL);
            break;
        case 4:
            printf("Restarting now.\n");
            fflush(stdout);
            esp_restart();
            break;
        default:
            break;
        }
    }
}

/**
 * @brief 壁纸初始化
 *
 * @param num 0 windows 1 天空
 */
void wallpaper_init(uint8_t num)
{
    static uint32_t init = false;
    static lv_obj_t *img = NULL;

    LV_IMG_DECLARE(logo)
    LV_IMG_DECLARE(sky)
    LV_IMG_DECLARE(leaf)
    LV_IMG_DECLARE(flower)

    if (init == false)
    {
        init = true;
        img = lv_img_create(lv_scr_act(), NULL);
        lv_obj_set_style_local_bg_color(img, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    }

    switch (num)
    {
    case WALLPAPER_WINDOWS:
        lv_img_set_src(img, &logo);
        break;
    case WALLPAPER_SKY:
        lv_img_set_src(img, &sky);
        break;
    case WALLPAPER_LEAF:
        lv_img_set_src(img, &leaf);
        break;
    case WALLPAPER_FLOWER:
        lv_img_set_src(img, &flower);
        break;
    default:
        break;
    }

    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
}

/**
 * @brief 展示开机动画
 *
 */
static void ui_start_up(void)
{
    lv_port_sem_take();

    group = Get_group();

    lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0, 139, 139));
    wallpaper_init(WALLPAPER_LEAF);

    /* Create a bar to update loading progress */
    lv_obj_t *bar = lv_bar_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, COLOR_BG);
    lv_obj_set_style_local_border_color(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_LONON);
    lv_obj_set_style_local_border_width(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, 2);
    lv_obj_set_size(bar, 230, 16);
    lv_obj_align(bar, NULL, LV_ALIGN_CENTER, 0, 70);
    lv_bar_set_range(bar, 0, 200);
    lv_bar_set_value(bar, 1, LV_ANIM_ON);

    lv_obj_t *label_loading_hint = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text_static(label_loading_hint, " ");
    lv_obj_set_style_local_text_font(label_loading_hint, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_18);
    lv_obj_set_style_local_text_color(label_loading_hint, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_align(label_loading_hint, bar, LV_ALIGN_OUT_BOTTOM_MID, -110, -50);
    lv_port_sem_give();

    /* Load resource from SD crad to PSARM */
    TickType_t tick = xTaskGetTickCount();
    for (size_t i = 0; i < 200; i++)
    {
        lv_port_sem_take();
        lv_bar_set_value(bar, i + 1, LV_ANIM_ON);
        lv_label_set_text_fmt(label_loading_hint, "Loading ... ...");
        lv_port_sem_give();
        /* Yeah, it's only because that makes ui more flexible */
        vTaskDelayUntil(&tick, pdMS_TO_TICKS(10));
    }

    lv_obj_del(bar);
    lv_obj_del(label_loading_hint);
    lv_port_sem_give();
}

/**
 * @brief 移动菜单
 *
 * @param bg_img
 */
static void list_move(void *bg_img)
{
    lv_anim_t lv_anim;
    lv_anim_init(&lv_anim);
    lv_anim_set_exec_cb(&lv_anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_var(&lv_anim, bg_img);
    lv_anim_set_time(&lv_anim, 600);
    lv_anim_set_values(&lv_anim, 100, 20);
    lv_anim_start(&lv_anim);
}

/**
 * @brief 展示主页
 *
 * @param sw 打开 true 关闭 false
 */
static void ui_menu(uint8_t sw)
{
    static uint32_t init = false;
    static uint32_t status_bar_init = false;
    static lv_obj_t *list_fun = NULL;
    static lv_obj_t *status_bar = NULL;
    static lv_obj_t *btn_wifi = NULL;
    static lv_obj_t *list_fun_option[LIST_MAX_FUN];
    static void *img_src[LIST_MAX_FUN] = {
        LV_SYMBOL_GPS,
        LV_SYMBOL_AUDIO,
        LV_SYMBOL_WIFI,
        LV_SYMBOL_SETTINGS,
        LV_SYMBOL_POWER,
    };
    static char list_fun_text[LIST_MAX_FUN][16] = {

        "Weather",
        "Music",
        "WIFI scan",
        "Setting",
        "Reset",
    };

    lv_port_sem_take();

    if (sw)
    {
        if (init == false)
        {
            init = true;

            if (status_bar_init == false)
            {
                status_bar_init = true;

                status_bar = lv_obj_create(lv_scr_act(), NULL);
                lv_obj_set_size(status_bar, 320, 50);
                lv_obj_set_style_local_bg_color(status_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BAR);
                lv_obj_set_style_local_radius(status_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
                lv_obj_set_style_local_border_width(status_bar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
                lv_obj_align(status_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

                btn_wifi = lv_btn_create(status_bar, NULL);
                btn_wifi2 = btn_wifi;
                lv_obj_set_width(btn_wifi, 50);
                lv_obj_set_click(btn_wifi, false);
                lv_obj_set_style_local_radius(btn_wifi, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 25);
                lv_obj_set_style_local_bg_color(btn_wifi, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, COLOR_BAR);
                lv_obj_set_style_local_border_color(btn_wifi, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, COLOR_BAR);
                lv_obj_set_style_local_value_font(btn_wifi, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, &font_bar_symbol);
                lv_obj_set_style_local_value_color(btn_wifi, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xd9e1f9));
                lv_obj_set_style_local_value_str(btn_wifi, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_EXTRA_WIFI_OFF);
                lv_obj_align(btn_wifi, NULL, LV_ALIGN_OUT_RIGHT_MID, -70, 0);
            }
            else
            {
                lv_obj_set_hidden(status_bar, false);
                lv_obj_set_hidden(btn_wifi, false);
            }

            list_fun = lv_list_create(lv_scr_act(), NULL);
            list_fun2 = list_fun;
            lv_obj_set_size(list_fun, 150, 200);
            lv_obj_align(list_fun, NULL, LV_ALIGN_IN_TOP_MID, -60, 20);

            for (size_t i = 0; i < LIST_MAX_FUN; i++)
            {
                list_fun_option[i] = lv_list_add_btn(list_fun, img_src[i], &list_fun_text[i][0]);
                lv_obj_set_event_cb(list_fun_option[i], list_event_handler);
            }

            lv_group_remove_all_objs(group);
            lv_group_set_editing(group, false);
            for (size_t i = 0; i < LIST_MAX_FUN; i++)
                lv_group_add_obj(group, list_fun_option[i]);

            list_move(list_fun);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            lv_obj_set_hidden(status_bar, true);
            lv_obj_set_hidden(btn_wifi, true);
            lv_list_clean(list_fun);
            lv_obj_del(list_fun);
        }
    }

    lv_port_sem_give();
}

/**
 * @brief Get the btn wifi obj object 获取wifi状态对象
 *
 * @return lv_obj_t*
 */
static lv_obj_t *get_btn_wifi_obj()
{
    return btn_wifi2;
}

/**
 * @brief Get the list fun obh object 获取功能列表对象
 *
 * @return lv_obj_t*
 */
static lv_obj_t *get_list_fun_obj()
{
    return list_fun2;
}

/**
 * @brief 播放开机声音
 *
 */
void play_start_up_sound()
{
    uint32_t mp3_index = 0;
    xQueueSend(play_queue, &mp3_index, NULL);
}

/**
 * @brief 页面任务
 *
 */
void main_page_task()
{
    static uint32_t run_http_get_time = false;
    uint32_t page_option_data;

    while (1)
    {
        if (xQueueReceive(main_page_queue, &page_option_data, portMAX_DELAY))
        {
            switch (page_option_data)
            {
            case PAGE_SHOW:
                wallpaper_init(WALLPAPER_LEAF);
                ui_menu(true);
                break;
            case PAGE_HIDDEN:
                ui_menu(false);
                break;
            case PAGE_HANDLE_1: // WIFI已连接
                wiif_connect_state = WIFI_CONNECT_OK;
                lv_obj_set_style_local_value_str(get_btn_wifi_obj(), LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_EXTRA_WIFI_MAX);

                if (run_http_get_time == false)
                {
                    run_http_get_time = true;
                    xTaskCreate(&http_get_time, "http_get_time", 4096, NULL, 5, NULL);
                }
                break;
            case PAGE_HANDLE_2: // WIFI已断开
                wiif_connect_state = WIFI_CONNECT_NULL;
                lv_obj_set_style_local_value_str(get_btn_wifi_obj(), LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_EXTRA_WIFI_OFF);
                break;
            default:
                break;
            }
        }
    }
}

esp_err_t nvs_init()
{
    /*!< Initialize NVS */
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    return ret;
}

/**
 * @brief 开机自动连接wifi
 *
 */
void connect_wifi()
{
    char get_ssid[32];
    char get_pwd[64];

    if (get_wifi_info((uint8_t *)get_ssid, (uint8_t *)get_pwd) == ESP_OK)
    {
        ESP_LOGI(TAG, "ssid信息有效 进行开机自动连接wifi ssid: %s pwd: %s", get_ssid, get_pwd);
        wifi_connect_init((uint8_t *)get_ssid, (uint8_t *)get_pwd);
    }
}

void app_main(void)
{
    /*!< Initialize NVS */
    ESP_ERROR_CHECK(nvs_init());

    /* Initialize LCD */
    ESP_ERROR_CHECK(bsp_lcd_init());

    /* Initialize LVGL */
    ESP_ERROR_CHECK(lvgl_init(LVGL_SCR_SIZE / 8, LV_BUF_ALLOC_INTERNAL));

    /* Init LVGL file system API */
    lv_port_fs_init();

    /* Initialize storage */
    storage_init();

    /* Initialize pwm bck */
    pwm_init();

    /* Initialize music */
    music_init();

    /* Start UI */
    ui_start_up();

    /* ui_menu UI */
    ui_menu(true);

    /* Initialize encoder */
    encoder_init();

    /* Initialize done */
    play_start_up_sound();

    /* Initialize page */
    cql_page_init(MAIN_PAGE_TASK_NUM);

    /* auto connect wiif */
    connect_wifi();

    ESP_LOGI(TAG, "[APP] Free internal memory: %d bytes", esp_get_free_internal_heap_size());
    ESP_LOGI(TAG, "[APP] Free all memory: %d bytes", esp_get_free_heap_size());
}
