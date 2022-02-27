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
#include "music_page.h"
#include "main_page.h"
#include "lv_symbol_extra_def.h"
#include "weather_page.h"
#include "lvgl_port.h"
#include "lv_port_fs.h"
#include "music_page.h"
#include "audio.h"
#include "task_mag.h"

#define TAG "MUSIC_PAGE"

extern xQueueHandle play_queue;
xQueueHandle music_page_queue = NULL;

lv_group_t *group;
extern lv_group_t *Get_group(void);
static lv_obj_t *list_fun2;
static lv_obj_t *btn_pause_pause2 = NULL;

extern xQueueHandle main_page_queue;
void set_btn_pause_pause_icon(const char *value);

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
 * @brief 功能列表事件回调
 *
 * @param obj 列表对象
 * @param event 事件
 */
static void list_event_handler(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;
    uint32_t mp3_index = 0;
    extern int play_flag;

    if (event == LV_EVENT_CLICKED)
    {
        printf("Clicked: %s\n", lv_list_get_btn_text(obj));
        uint32_t index = lv_list_get_btn_index(get_list_fun_obj(), obj);
        printf("lv_list_get_btn_index = %d\r\n", index);

        set_btn_pause_pause_icon(LV_SYMBOL_PAUSE);

        play_flag = 1;
        mp3_index = index;
        xQueueSend(play_queue, &mp3_index, NULL);
    }
}

/**
 * @brief 设置播放按钮图标
 *
 * @param value
 */
void set_btn_pause_pause_icon(const char *value)
{
    lv_obj_set_style_local_value_str(btn_pause_pause2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, value);
}

/**
 * @brief 暂停播放按钮
 *
 * @param obj 列表对象
 * @param event 列表事件
 */
static void btn_play_pause_cb(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;
    extern int play_flag;

    if (event == LV_EVENT_CLICKED)
    {
        play_flag = 0;
        set_btn_pause_pause_icon(LV_SYMBOL_STOP);
    }
}

/**
 * @brief 退出按钮
 *
 * @param obj 列表对象
 * @param event 列表事件
 */
static void btn_exit_cb(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;

    if (event == LV_EVENT_CLICKED)
    {
        extern int play_flag;
        play_flag = 0;

        page_option_data = PAGE_EXIT;
        xQueueSend(music_page_queue, &page_option_data, NULL);
    }
}

#define MUSIC_LIST_MAX_FUN 5

/**
 * @brief 展示主页
 *
 * @param sw 打开 true 关闭 false
 */
static void ui_menu(uint8_t sw)
{
    static uint32_t init = false;
    static lv_obj_t *list_fun = NULL;
    static lv_obj_t *btn_exit = NULL;
    static lv_obj_t *btn_pause_pause = NULL;
    static lv_obj_t *list_fun_option[MUSIC_LIST_MAX_FUN];
    static void *img_src[MUSIC_LIST_MAX_FUN];

    for (size_t i = 0; i < MUSIC_LIST_MAX_FUN; i++)
        img_src[i] = LV_SYMBOL_AUDIO;

    extern const char audio_list[AUDIO_MAX_PLAY_LIST][64];
    uint32_t music_num;
    music_num = AUDIO_MAX_PLAY_LIST;

    static char list_fun_text[MUSIC_LIST_MAX_FUN][32];

    memset(list_fun_text, 0, sizeof(list_fun_text));

    for (size_t i = 0; i < AUDIO_MAX_PLAY_LIST; i++)
        memcpy(&list_fun_text[i][0], &audio_list[i][8], strlen(&audio_list[i][8]));

    lv_port_sem_take();

    if (sw)
    {
        if (init == false)
        {
            init = true;

            list_fun = lv_list_create(lv_scr_act(), NULL);
            list_fun2 = list_fun;
            lv_obj_set_size(list_fun, 280, 170);
            lv_obj_align(list_fun, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);

            for (size_t i = 0; i < music_num; i++)
            {
                list_fun_option[i] = lv_list_add_btn(list_fun, img_src[i], &list_fun_text[i][0]);
                lv_obj_set_event_cb(list_fun_option[i], list_event_handler);
            }

            lv_group_remove_all_objs(group);
            lv_group_set_editing(group, false);
            for (size_t i = 0; i < music_num; i++)
                lv_group_add_obj(group, list_fun_option[i]);

            btn_exit = lv_btn_create(lv_scr_act(), NULL);
            lv_obj_set_size(btn_exit, 100, 35);
            lv_obj_set_event_cb(btn_exit, btn_exit_cb);
            lv_obj_align(btn_exit, list_fun, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
            lv_obj_set_style_local_value_str(btn_exit, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_HOME);
            lv_obj_set_style_local_bg_color(btn_exit, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_THEME);
            lv_group_add_obj(group, btn_exit);

            btn_pause_pause = lv_btn_create(lv_scr_act(), NULL);
            btn_pause_pause2 = btn_pause_pause;
            lv_obj_set_size(btn_pause_pause, 100, 35);
            lv_obj_set_event_cb(btn_pause_pause, btn_play_pause_cb);
            lv_obj_align(btn_pause_pause, list_fun, LV_ALIGN_OUT_BOTTOM_LEFT, 120, 10);
            lv_obj_set_style_local_value_str(btn_pause_pause, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_STOP);
            lv_obj_set_style_local_bg_color(btn_pause_pause, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_THEME);
            lv_group_add_obj(group, btn_pause_pause);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            lv_list_clean(list_fun);
            lv_obj_del(list_fun);
            lv_obj_del(btn_exit);
            lv_obj_del(btn_pause_pause);
        }
    }

    lv_port_sem_give();
}

/**
 *
 * @brief 页面任务
 *
 */
void music_page_task()
{
    uint32_t page_option_data;

    while (1)
    {
        if (xQueueReceive(music_page_queue, &page_option_data, portMAX_DELAY))
        {
            switch (page_option_data)
            {
            case PAGE_SHOW:
                ui_menu(true);
                break;
            case PAGE_EXIT:
                ui_menu(false);

                vTaskDelay(200 / portTICK_RATE_MS);
                page_option_data = PAGE_SHOW;
                xQueueSend(main_page_queue, &page_option_data, NULL);

                cql_page_dele(MUSIC_PAGE_TASK_NUM);
                break;
            default:

                break;
            }
        }
    }
}
