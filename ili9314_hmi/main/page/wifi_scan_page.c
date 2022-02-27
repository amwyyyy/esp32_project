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
#include "task_mag.h"

xQueueHandle wifi_scan_page_queue = NULL;
extern xQueueHandle main_page_queue;
extern lv_group_t *group;
char choice_ssid[32];
char input_pwd[64];
static lv_obj_t *textarea_pwd2 = NULL;

static lv_obj_t *get_input_pwd_obj(void);

#define TAG "WIFI_SCAN_PAGE"

/**
 * @brief Get the wifi scan page ssid info object 获取 wifi_scan_page 页面的当前 ssid 与 密码信息
 *
 * @param ssid 输出 ssid
 * @param pwd 输出 密码
 * @return uint8_t true 有效信息 false 无效信息
 */
uint8_t get_wifi_scan_page_ssid_info(char *ssid, char *pwd)
{
    memcpy(ssid, choice_ssid, strlen(choice_ssid));
    memcpy(pwd, input_pwd, strlen(input_pwd));
    return true;
}

/**
 * @brief ssid列表点击事件
 *
 * @param obj 列表对象
 * @param event 列表事件
 */
static void list_event_handler(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;

    if (event == LV_EVENT_CLICKED)
    {
        memset(choice_ssid, 0, sizeof(choice_ssid));
        memcpy(choice_ssid, lv_list_get_btn_text(obj), strlen(lv_list_get_btn_text(obj)));
        printf("Clicked: %s\n", choice_ssid);
        page_option_data = PAGE_HANDLE_1;
        xQueueSend(wifi_scan_page_queue, &page_option_data, NULL);
    }
}

/**
 * @brief 退出按钮事件回调
 *
 * @param obj 按钮对象
 * @param event 按钮事件
 */
static void btn_exit_cb(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;

    if (event == LV_EVENT_PRESSED)
    {
        page_option_data = PAGE_EXIT;
        xQueueSend(wifi_scan_page_queue, &page_option_data, NULL);
    }
}

/**
 * @brief 展示wifi列表
 *
 * @param sw 打开 true 关闭 false
 */
static void show_wifi_list(uint8_t sw)
{
    static uint8_t init = false;
    static lv_obj_t *list_ssid_option[LIST_MAX_SSID];
    static lv_obj_t *list_ssid = NULL;
    static lv_obj_t *btn_exit = NULL;
    static lv_obj_t *label_ap = NULL;
    extern wifi_ap_record_t *wifi_ap_record;
    extern uint16_t wifi_ap_record_num;
    char ssid_name[64];

    lv_port_sem_take();

    if (sw)
    {
        if (init == false)
        {
            init = true;

            if (wifi_ap_record_num > LIST_MAX_SSID)
                wifi_ap_record_num = LIST_MAX_SSID;

            if (wifi_ap_record == NULL)
                wifi_ap_record_num = 0;

            ESP_LOGW(TAG, "SSID列表展示数量(%d)", wifi_ap_record_num);

            list_ssid = lv_list_create(lv_scr_act(), NULL);
            lv_obj_set_size(list_ssid, 280, 170);
            lv_obj_align(list_ssid, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);

            btn_exit = lv_btn_create(lv_scr_act(), NULL);
            lv_obj_set_size(btn_exit, 100, 35);
            lv_obj_set_event_cb(btn_exit, btn_exit_cb);
            lv_obj_align(btn_exit, list_ssid, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
            lv_obj_set_style_local_value_str(btn_exit, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_HOME);
            lv_obj_set_style_local_bg_color(btn_exit, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_THEME);

            label_ap = lv_label_create(lv_scr_act(), NULL);
            lv_obj_set_style_local_text_font(label_ap, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_18);
            lv_obj_set_style_local_text_color(label_ap, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            lv_obj_align(label_ap, list_ssid, LV_ALIGN_OUT_BOTTOM_LEFT, 120, 10);
            lv_label_set_text_fmt(label_ap, "scan ap: %d", wifi_ap_record_num);

            for (size_t i = 0; i < wifi_ap_record_num; i++)
            {
                sprintf(ssid_name, "%s", (char *)(wifi_ap_record[i].ssid));
                list_ssid_option[i] = lv_list_add_btn(list_ssid, LV_SYMBOL_WIFI, ssid_name);
                lv_obj_set_event_cb(list_ssid_option[i], list_event_handler);
            }

            lv_group_remove_all_objs(group);
            lv_group_set_editing(group, false);
            for (size_t i = 0; i < wifi_ap_record_num; i++)
                lv_group_add_obj(group, list_ssid_option[i]);

            if (wifi_ap_record)
            {
                free(wifi_ap_record);
                wifi_ap_record = NULL;
            }
            lv_group_add_obj(group, btn_exit);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;
            
            lv_list_clean(list_ssid);
            lv_obj_del(list_ssid);
            lv_obj_del(btn_exit);
            lv_obj_del(label_ap);
        }
    }

    lv_port_sem_give();
}

/**
 * @brief 键盘事件回调
 *
 * @param obj 键盘对象
 * @param event 键盘事件
 */
static void keyboard_pwd_event_cb(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;
    lv_keyboard_def_event_cb(obj, event);

    switch (event)
    {
    case LV_EVENT_CANCEL:
        ESP_LOGW(TAG, "键盘 CANCEL 事件(%d)", event);
        page_option_data = PAGE_EXIT;
        xQueueSend(wifi_scan_page_queue, &page_option_data, NULL);
        break;
    case LV_EVENT_APPLY:
        ESP_LOGW(TAG, "键盘 APPLY 事件(%d)", event);

        memset(input_pwd, 0, sizeof(input_pwd));
        memcpy(input_pwd, lv_textarea_get_text(get_input_pwd_obj()), strlen(lv_textarea_get_text(get_input_pwd_obj())));
        ESP_LOGW(TAG, "输入密码 %s", input_pwd);

        page_option_data = PAGE_HANDLE_2;
        xQueueSend(wifi_scan_page_queue, &page_option_data, NULL);
        break;
    default:
        break;
    }
}

/**
 * @brief ssid密码输入键盘控件
 *
 * @param sw 打开 true 关闭 false
 * @param ssid 提示输入密码的ssid
 */
static void keyboard_control(uint8_t sw, char *ssid)
{
    static uint32_t init = false;
    static lv_obj_t *textarea_pwd = NULL;
    static lv_obj_t *label_tips = NULL;
    static lv_obj_t *keyboard_pwd = NULL;

    lv_port_sem_take();
    if (sw)
    {
        if (init == false)
        {
            if (!ssid)
                return;

            init = true;

            textarea_pwd = lv_textarea_create(lv_scr_act(), NULL);
            textarea_pwd2 = textarea_pwd;
            lv_textarea_set_text(textarea_pwd, "");
            lv_obj_set_size(textarea_pwd, 250, 35);
            lv_obj_align(textarea_pwd, NULL, LV_ALIGN_CENTER, 0, -40);

            label_tips = lv_label_create(lv_scr_act(), NULL);
            lv_obj_set_style_local_text_font(label_tips, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_18);
            lv_obj_set_style_local_text_color(label_tips, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
            lv_obj_align(label_tips, textarea_pwd, LV_ALIGN_IN_TOP_LEFT, 0, -30);
            lv_label_set_text_fmt(label_tips, "%s - pwd:", ssid);

            keyboard_pwd = lv_keyboard_create(lv_scr_act(), NULL);
            lv_obj_set_size(keyboard_pwd, 320, 120);
            lv_obj_set_pos(keyboard_pwd, 0, 0);
            lv_obj_align(keyboard_pwd, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
            lv_keyboard_set_cursor_manage(keyboard_pwd, true);
            lv_obj_set_event_cb(keyboard_pwd, keyboard_pwd_event_cb);
            lv_keyboard_set_textarea(keyboard_pwd, textarea_pwd);

            lv_group_remove_all_objs(group);
            lv_group_set_editing(group, true);
            lv_group_add_obj(group, keyboard_pwd);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;
            
            lv_obj_del(keyboard_pwd);
            lv_obj_del(textarea_pwd);
            lv_obj_del(label_tips);
        }
    }
    lv_port_sem_give();
}

/**
 * @brief Get the input pwd obj object 获取输入密码文本框对象
 *
 * @return lv_obj_t* 返回输入密码文本框对象
 */
static lv_obj_t *get_input_pwd_obj()
{
    return textarea_pwd2;
}

/**
 * @brief 载入动画
 *
 * @param sw 打开 true 关闭 false（允许重复打开可以更新提示语）
 * @param hint 提示语
 */
static void load_control(uint8_t sw, char *hint)
{
    static uint8_t init = false;
    static lv_obj_t *preload1 = NULL;
    static lv_obj_t *label_loading_hint = NULL;

    lv_port_sem_take();
    if (sw)
    {
        if (init == false)
        {
            if (!hint)
                return;

            init = true;
            preload1 = lv_spinner_create(lv_scr_act(), NULL);
            lv_obj_set_size(preload1, 100, 100);
            lv_obj_align(preload1, NULL, LV_ALIGN_CENTER, 0, 0);
            lv_spinner_set_type(preload1, LV_SPINNER_TYPE_FILLSPIN_ARC);

            label_loading_hint = lv_label_create(lv_scr_act(), NULL);
            lv_obj_set_style_local_text_font(label_loading_hint, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_18);
            lv_obj_set_style_local_text_color(label_loading_hint, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            lv_obj_align(label_loading_hint, preload1, LV_ALIGN_IN_BOTTOM_LEFT, 0, 30);
            lv_label_set_text_fmt(label_loading_hint, hint);
        }
        else
        {
            lv_label_set_text_fmt(label_loading_hint, hint);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            lv_obj_del(preload1);
            lv_obj_del(label_loading_hint);
        }
    }

    lv_port_sem_give();
}

/**
 * @brief 消息弹窗选择回调
 *
 * @param obj 列表对象
 * @param event 列表事件
 */
static void msgbox_event_handler(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;

    if (event == LV_EVENT_VALUE_CHANGED)
    {
        ESP_LOGW(TAG, "选定的按钮序号 %u", lv_msgbox_get_active_btn(obj));
        page_option_data = PAGE_HANDLE_3;
        xQueueSend(wifi_scan_page_queue, &page_option_data, NULL);
    }
}

/**
 * @brief 消息弹窗
 *
 * @param sw 打开 true 关闭 false
 * @param hint 展示文本
 */
static void msgbox_control(uint8_t sw, char *hint)
{
    static uint8_t init = false;
    static const char *btn_str[] = {"OK", ""};
    static lv_obj_t *msgbox = NULL;

    lv_port_sem_take();
    if (sw)
    {
        if (init == false)
        {
            if (!hint)
                return;

            init = true;

            msgbox = lv_msgbox_create(lv_scr_act(), NULL);
            lv_obj_set_size(msgbox, 280, 150);
            lv_msgbox_set_text(msgbox, hint);
            lv_msgbox_add_btns(msgbox, btn_str);
            lv_obj_set_event_cb(msgbox, msgbox_event_handler);
            lv_obj_align(msgbox, NULL, LV_ALIGN_CENTER, 0, 0);

            lv_group_remove_all_objs(group);
            lv_group_set_editing(group, true);
            lv_group_add_obj(group, msgbox);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            lv_obj_del(msgbox);
        }
    }

    lv_port_sem_give();
}

/**
 * @brief 页面任务
 *
 */
void wifi_scan_page_task()
{
    uint32_t page_option_data;
    extern EventGroupHandle_t wifi_event_group;
    uint32_t connect_ret;

    while (1)
    {
        if (xQueueReceive(wifi_scan_page_queue, &page_option_data, portMAX_DELAY))
        {
            switch (page_option_data)
            {
            case PAGE_SHOW:
                ESP_LOGW(TAG, "页面展示");
                show_wifi_list(false);
                keyboard_control(false, NULL);
                load_control(false, NULL);

                load_control(true, "wifi scan ... ...");
                wifi_scan_init();

                connect_ret = xEventGroupWaitBits(wifi_event_group, SCAN_LIST_DONE_BIT, true, false, portMAX_DELAY);
                xEventGroupClearBits(wifi_event_group, connect_ret);

                load_control(false, NULL);
                show_wifi_list(true);
                break;
            case PAGE_EXIT:
                ESP_LOGW(TAG, "页面退出");
                connect_wifi();
                show_wifi_list(false);
                keyboard_control(false, NULL);
                load_control(false, NULL);
                msgbox_control(false, NULL);

                vTaskDelay(200 / portTICK_RATE_MS);
                page_option_data = PAGE_SHOW;
                xQueueSend(main_page_queue, &page_option_data, NULL);

                cql_page_dele(WIFI_SCAN_TASK_NUM);
                break;
            case PAGE_HANDLE_1: //展示ssid密码输入键盘
                show_wifi_list(false);
                keyboard_control(true, choice_ssid);
                break;
            case PAGE_HANDLE_2: //连接wifi展示等待动画
                keyboard_control(false, NULL);
                load_control(true, "coonnect wifi ... ...");
                xEventGroupSetBits(wifi_event_group, TRY_CONNECT_WIFI_BIT);
                connect_ret = xEventGroupWaitBits(wifi_event_group, CONNECT_DONE_BIT | CONNECT_FAIL_BIT, true, false, portMAX_DELAY);
                xEventGroupClearBits(wifi_event_group, connect_ret);
                load_control(false, NULL);

                if (connect_ret & CONNECT_DONE_BIT)
                {

                    char *string = heap_caps_malloc(sizeof(char) * 64, MALLOC_CAP_SPIRAM);
                    memset(string, 0, sizeof(char) * 64);
                    sprintf(string, "connect success.\r\nip: %s", get_ip_str());
                    msgbox_control(true, string);
                    set_wifi_info(choice_ssid, input_pwd);
                    free(string);
                }
                else
                {
                    char *string = heap_caps_malloc(sizeof(char) * 64, MALLOC_CAP_SPIRAM);
                    memset(string, 0, sizeof(char) * 64);
                    sprintf(string, "fail to connect \"%s\"", choice_ssid);
                    msgbox_control(true, string);
                }
                break;
            case PAGE_HANDLE_3: //连接结果选定
                msgbox_control(false, NULL);
                page_option_data = PAGE_EXIT;
                xQueueSend(wifi_scan_page_queue, &page_option_data, NULL);
                break;
            default:

                break;
            }
        }
    }
}
