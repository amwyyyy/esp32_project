#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "string.h"
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
#include "wifi_scan_page.h"
#include "wifi_scan.h"
#include "main_page.h"
#include "weather_page.h"
#include "setting_page.h"
#include "audio.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "cJSON.h"
#include "task_mag.h"
#include "pwm.h"

#define TAG "SETTING_PAGE"

xQueueHandle setting_page_queue = NULL;

extern lv_group_t *group;
extern xQueueHandle main_page_queue;
extern lv_group_t *Get_group(void);

static lv_obj_t *slider_vol2 = NULL;
static lv_obj_t *slider_label_vol2 = NULL;

static lv_obj_t *slider_bl2 = NULL;
static lv_obj_t *slider_label_bl2 = NULL;

static uint32_t slider_vol_value = 0;
static uint32_t slider_bl_value = 0;

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
        page_option_data = PAGE_EXIT;
        xQueueSend(setting_page_queue, &page_option_data, NULL);
    }
}

/**
 * @brief 音量滑条
 *
 * @param obj 列表对象
 * @param event 列表事件
 */
static void slider_vol_cb(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;

    if (event == LV_EVENT_VALUE_CHANGED)
    {
        page_option_data = PAGE_HANDLE_1;
        xQueueSend(setting_page_queue, &page_option_data, NULL);
    }
}

/**
 * @brief 背光滑条
 *
 * @param obj 列表对象
 * @param event 列表事件
 */
static void slider_bl_cb(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;

    if (event == LV_EVENT_VALUE_CHANGED)
    {
        page_option_data = PAGE_HANDLE_2;
        xQueueSend(setting_page_queue, &page_option_data, NULL);
    }
}

static void lv_ex_slider(uint8_t sw)
{
    static uint32_t init = false;
    static lv_obj_t *slider_vol = NULL;
    static lv_obj_t *slider_label_vol = NULL;
    static lv_obj_t *slider_bl = NULL;
    static lv_obj_t *slider_label_bl = NULL;
    static lv_style_t slider_indic_style;
    static lv_style_t slider_knob_style;
    static lv_obj_t *btn_exit = NULL;

    lv_port_sem_take();

    if (sw == true)
    {
        if (init == false)
        {
            init = true;

            lv_style_init(&slider_indic_style);
            lv_style_set_bg_color(&slider_indic_style, LV_STATE_DEFAULT, LV_COLOR_LONON);

            lv_style_init(&slider_knob_style);
            lv_style_set_bg_color(&slider_knob_style, LV_STATE_DEFAULT, LV_COLOR_SILVER);

            // vol
            slider_vol = lv_slider_create(lv_scr_act(), NULL);
            slider_vol2 = slider_vol;
            lv_obj_set_event_cb(slider_vol, slider_vol_cb);
            lv_obj_add_style(slider_vol, LV_SLIDER_PART_INDIC, &slider_indic_style);
            lv_obj_add_style(slider_vol, LV_SLIDER_PART_KNOB, &slider_knob_style);

            lv_obj_set_width(slider_vol, 190);
            lv_obj_set_height(slider_vol, 8);

            lv_obj_align(slider_vol, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 20);
            lv_slider_set_range(slider_vol, 0, 10);

            slider_label_vol = lv_label_create(lv_scr_act(), NULL);
            slider_label_vol2 = slider_label_vol;
            lv_label_set_text(slider_label_vol, "vol: 100");
            lv_obj_set_auto_realign(slider_label_vol, true);
            lv_obj_align(slider_label_vol, slider_vol, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
            lv_obj_set_style_local_text_font(slider_label_vol, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_bold_20);

            // bl
            slider_bl = lv_slider_create(lv_scr_act(), NULL);
            slider_bl2 = slider_bl;
            lv_obj_set_event_cb(slider_bl, slider_bl_cb);
            lv_obj_add_style(slider_bl, LV_SLIDER_PART_INDIC, &slider_indic_style);
            lv_obj_add_style(slider_bl, LV_SLIDER_PART_KNOB, &slider_knob_style);

            lv_obj_set_width(slider_bl, 190);
            lv_obj_set_height(slider_bl, 8);

            lv_obj_align(slider_bl, slider_vol, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 30);
            lv_slider_set_range(slider_bl, 0, 10);

            slider_label_bl = lv_label_create(lv_scr_act(), NULL);
            slider_label_bl2 = slider_label_bl;
            lv_label_set_text(slider_label_bl, "bl: 100");
            lv_obj_set_auto_realign(slider_label_bl, true);
            lv_obj_align(slider_label_bl, slider_bl, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
            lv_obj_set_style_local_text_font(slider_label_bl, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_bold_20);

            // exit
            btn_exit = lv_btn_create(lv_scr_act(), NULL);
            lv_obj_set_size(btn_exit, 100, 35);
            lv_obj_set_event_cb(btn_exit, btn_exit_cb);
            lv_obj_align(btn_exit, slider_bl, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
            lv_obj_set_style_local_value_str(btn_exit, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_HOME);
            lv_obj_set_style_local_bg_color(btn_exit, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_THEME);
            lv_group_add_obj(group, btn_exit);

            // group
            lv_group_remove_all_objs(group);
            lv_group_set_editing(group, false);
            lv_group_add_obj(group, slider_vol);
            lv_group_add_obj(group, slider_bl);
            lv_group_add_obj(group, btn_exit);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            lv_obj_del(slider_vol);
            lv_obj_del(slider_label_vol);
            lv_obj_del(slider_bl);
            lv_obj_del(slider_label_bl);
            lv_obj_del(btn_exit);
        }
    }

    lv_port_sem_give();
}

/**
 *
 * @brief 页面任务
 *
 */
void setting_page_task()
{
    uint32_t get_vol = 0;
    uint32_t get_bl = 0;
    uint32_t page_option_data;
    char string[16];
    while (1)
    {
        if (xQueueReceive(setting_page_queue, &page_option_data, portMAX_DELAY))
        {
            switch (page_option_data)
            {
            case PAGE_SHOW:
                lv_ex_slider(true);

                // vol
                get_setting_vol(&get_vol);
                lv_slider_set_value(slider_vol2, get_vol, true);

                page_option_data = PAGE_HANDLE_1;
                xQueueSend(setting_page_queue, &page_option_data, NULL);

                // bl
                get_setting_bl(&get_bl);
                lv_slider_set_value(slider_bl2, get_bl, true);

                page_option_data = PAGE_HANDLE_2;
                xQueueSend(setting_page_queue, &page_option_data, NULL);
                break;
            case PAGE_EXIT:
                // vol
                audio_setting_vol(lv_slider_get_value(slider_vol2));
                set_setting_vol(lv_slider_get_value(slider_vol2));
                get_setting_vol(&get_vol);
                audio_setting_vol(get_vol * 10);

                // bl
                set_setting_bl(lv_slider_get_value(slider_bl2));

                lv_ex_slider(false);

                vTaskDelay(200 / portTICK_RATE_MS);
                page_option_data = PAGE_SHOW;
                xQueueSend(main_page_queue, &page_option_data, NULL);

                cql_page_dele(SETTING_PAGE_TASK_NUM);
                break;
            case PAGE_HANDLE_1:
                memset(string, 0, sizeof(string));
                sprintf(string, "vol: %d", lv_slider_get_value(slider_vol2) * 10);
                lv_label_set_text(slider_label_vol2, string);
                break;
            case PAGE_HANDLE_2:
                memset(string, 0, sizeof(string));
                sprintf(string, "bl: %d", lv_slider_get_value(slider_bl2) * 10);
                lv_label_set_text(slider_label_bl2, string);

                set_bl_pwm(lv_slider_get_value(slider_bl2));
                break;
            default:

                break;
            }
        }
    }
}
