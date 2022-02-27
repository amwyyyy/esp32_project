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

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "cJSON.h"
#include "task_mag.h"
#include "http_time.h"

#define TAG "WEATHER_PAGE"

#define WEATHER_MOVE_SW 0

#define WEATHER_DISPLAY_ICO 0  //显示天气图标
#define WEATHER_DISPLAY_TIME 1 //显示时间

static uint32_t weather_display = WEATHER_DISPLAY_ICO;

xQueueHandle weather_page_queue = NULL;

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "api.yytianqi.com"
#define WEB_PORT "80"
#define WEB_PATH "/observe?city=CH280101&key=3khp20k2dqjafks2"

#define WEATHER_TEXT_COLOR LV_COLOR_WHITE

static const char *REQUEST = "GET " WEB_PATH " HTTP/1.0\r\n"
                             "Host: " WEB_SERVER ":" WEB_PORT "\r\n"
                             "User-Agent: esp-idf/1.0 esp32\r\n"
                             "\r\n";

static void *weather_update_daily_icon(uint32_t num);
static void display_weather(uint8_t sw, weather_data_t *weather_data);
lv_obj_t *weather_icon_display(uint8_t sw, void *_data);
static void weather_move(uint8_t sw, void *bg_img);
static void *_data = NULL;

extern lv_group_t *group;
extern xQueueHandle main_page_queue;
void get_hour_min_week_str(char *hour_min, uint32_t *week);

static void http_get_weather(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r = 0;
    char *recv_buf;

    while (1)
    {
        int err = getaddrinfo(WEB_SERVER, WEB_PORT, &hints, &res);

        if (err != 0 || res == NULL)
        {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        /* Code to print the resolved IP.

           Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */
        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

        s = socket(res->ai_family, res->ai_socktype, 0);
        if (s < 0)
        {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... allocated socket");

        if (connect(s, res->ai_addr, res->ai_addrlen) != 0)
        {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            freeaddrinfo(res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);

        if (write(s, REQUEST, strlen(REQUEST)) < 0)
        {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");

        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                       sizeof(receiving_timeout)) < 0)
        {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... set socket receiving timeout success");

        /* Read HTTP response */
        recv_buf = heap_caps_malloc(1024, MALLOC_CAP_SPIRAM);
        if (recv_buf)
        {
            memset(recv_buf, 0, 1024);
            uint32_t offset;
            offset = 0;

            do
            {
                r = read(s, (void *)&recv_buf[offset], 1024 - 1);
                offset += r;
            } while (r > 0);

            for (size_t i = 0; i < 1024; i++)
            {
                if (recv_buf[i] == '{')
                {
                    cJSON *json, *json_key1, *json_key2;
                    weather_data_t weather_data;
                    memset(&weather_data, 0, sizeof(weather_data));

                    json = cJSON_Parse(&recv_buf[i]);
                    if (json)
                    {
                        json_key1 = cJSON_GetObjectItem(json, "data");
                        if (json_key1)
                        {
                            json_key2 = cJSON_GetObjectItem(json_key1, "numtq");
                            if (json_key2)
                                weather_data.numtq = atoi(cJSON_GetStringValue(json_key2));

                            json_key2 = cJSON_GetObjectItem(json_key1, "cityId");
                            if (json_key2)
                                memcpy(&weather_data.cityId, cJSON_GetStringValue(json_key2), strlen(cJSON_GetStringValue(json_key2)));

                            json_key2 = cJSON_GetObjectItem(json_key1, "qw");
                            if (json_key2)
                                memcpy(&weather_data.qw, cJSON_GetStringValue(json_key2), strlen(cJSON_GetStringValue(json_key2)));

                            json_key2 = cJSON_GetObjectItem(json_key1, "sd");
                            if (json_key2)
                                memcpy(&weather_data.sd, cJSON_GetStringValue(json_key2), strlen(cJSON_GetStringValue(json_key2)));

                            json_key2 = cJSON_GetObjectItem(json_key1, "lastUpdate");
                            if (json_key2)
                                memcpy(&weather_data.lastUpdate, cJSON_GetStringValue(json_key2) + 5, strlen(cJSON_GetStringValue(json_key2)));

                            display_weather(true, &weather_data);

                            if (_data)
                            {
                                weather_move(false, NULL);
                                weather_icon_display(false, NULL);

                                free(_data);
                                _data = NULL;
                            }

                            /**
                             * @brief 当前为显示天气图标才刷新
                             *
                             */
                            if (weather_display == WEATHER_DISPLAY_ICO)
                            {
                                _data = weather_update_daily_icon(weather_data.numtq);
                                weather_move(true, weather_icon_display(true, _data));
                            }
                        }
                        cJSON_Delete(json);
                    }
                    break;
                }
            }

            printf("%s\r\n\r\n", recv_buf);
            free(recv_buf);
        }

        ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", r, errno);
        close(s);
        for (size_t i = 0; i < 3600; i++)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        ESP_LOGI(TAG, "Starting again!");
    }
}

static void display_weather(uint8_t sw, weather_data_t *weather_data)
{
    static uint32_t init = false;
    static lv_obj_t *lv_label_temp;
    static lv_obj_t *lv_label_humidity;
    static lv_obj_t *lv_label_local;
    static lv_obj_t *lv_label_weather_describe;

    char str[32];
    char weather_describe[32];

    memset(str, 0, sizeof(str));
    memset(weather_describe, 0, sizeof(weather_describe));

    if (weather_data)
    {
        printf("\r\n\r\n天气现象编码 = %d\r\n", weather_data->numtq);
        printf("城市ID = %s\r\n", weather_data->cityId);
        printf("当前气温 = %s°C\r\n", weather_data->qw);
        printf("当前湿度 = %s%%\r\n", weather_data->sd);
        printf("实况更新时间 = %s\r\n\r\n", weather_data->lastUpdate);

        switch (weather_data->numtq)
        {
        case 0: //晴
            strcpy(weather_describe, "Sunny");
            break;
        case 1: //多云
            strcpy(weather_describe, "Cloudy");
            break;
        case 2: //阴
            strcpy(weather_describe, "Overcast");
            break;
        case 3: //阵雨
            strcpy(weather_describe, "Shower");
            break;
        case 7: //小雨
            strcpy(weather_describe, "Light rain");
            break;
        case 8: //中雨
            strcpy(weather_describe, "Moderate rain");
            break;
        case 9: //大雨
            strcpy(weather_describe, "Heavy rain");
            break;
        case 10: //暴雨
            strcpy(weather_describe, "Storm");
            break;
        case 11: //大暴雨
            strcpy(weather_describe, "Heavy storm");
            break;
        case 12: //特大暴雨
            strcpy(weather_describe, "Severe storm");
            break;
        case 14: //小雪
            strcpy(weather_describe, "Light snow");
            break;
        case 15: //中雪
            strcpy(weather_describe, "Moderate snow");
            break;
        case 16: //大雪
            strcpy(weather_describe, "Heavy snow");
            break;
        case 17: //暴雪
            strcpy(weather_describe, "Snowstorm");
            break;
        default:
            strcpy(weather_describe, "Unknown");
            break;
        }
    }

    lv_port_sem_take();

    if (sw)
    {
        if (init == false)
        {
            init = true;

            lv_label_temp = lv_label_create(lv_scr_act(), NULL);
            sprintf(str, "%s°C", weather_data->qw);
            lv_label_set_text(lv_label_temp, str);
            lv_obj_set_style_local_text_color(lv_label_temp, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, WEATHER_TEXT_COLOR);
            lv_obj_set_style_local_text_font(lv_label_temp, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_bold_72);
            lv_obj_align(lv_label_temp, NULL, LV_ALIGN_CENTER, -65, 65);

            lv_label_humidity = lv_label_create(lv_scr_act(), NULL);
            sprintf(str, "%s%%", weather_data->sd);
            lv_label_set_text(lv_label_humidity, str);
            lv_obj_set_style_local_text_color(lv_label_humidity, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, WEATHER_TEXT_COLOR);
            lv_obj_set_style_local_text_font(lv_label_humidity, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_bold_20);
            lv_obj_align(lv_label_humidity, lv_label_temp, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -10);

            lv_label_local = lv_label_create(lv_scr_act(), NULL);

            if (strstr(weather_data->cityId, "CH280801"))
                sprintf(str, "%s", "shunde");
            else
            {
                if (strstr(weather_data->cityId, "CH280101"))
                    sprintf(str, "%s", "guangzhou");
                else
                    sprintf(str, "%s", "unknow");
            }

            lv_label_set_text(lv_label_local, str);
            lv_obj_set_style_local_text_color(lv_label_local, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, WEATHER_TEXT_COLOR);
            lv_obj_set_style_local_text_font(lv_label_local, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_bold_20);
            lv_obj_align(lv_label_local, lv_label_temp, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -30);

            lv_label_weather_describe = lv_label_create(lv_scr_act(), NULL);
            lv_label_set_text(lv_label_weather_describe, weather_describe);
            lv_obj_set_style_local_text_color(lv_label_weather_describe, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, WEATHER_TEXT_COLOR);
            lv_obj_set_style_local_text_font(lv_label_weather_describe, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_bold_20);
            lv_obj_align(lv_label_weather_describe, lv_label_temp, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -50);
        }
        else
        {
            // lv_label_temp
            sprintf(str, "%s°C", weather_data->qw);
            lv_label_set_text(lv_label_temp, str);
            lv_obj_align(lv_label_temp, NULL, LV_ALIGN_CENTER, -65, 65);

            // lv_label_humidity
            sprintf(str, "%s%%", weather_data->sd);
            lv_label_set_text(lv_label_humidity, str);
            lv_obj_align(lv_label_humidity, lv_label_temp, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -10);

            // lv_label_local
            if (strstr(weather_data->cityId, "CH280801"))
                sprintf(str, "%s", "shunde");
            else
            {
                if (strstr(weather_data->cityId, "CH280101"))
                    sprintf(str, "%s", "guangzhou");
                else
                    sprintf(str, "%s", "unknow");
            }

            lv_label_set_text(lv_label_local, str);
            lv_obj_align(lv_label_local, lv_label_temp, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -30);

            // lv_label_weather_describe
            lv_label_set_text(lv_label_weather_describe, weather_describe);
            lv_obj_align(lv_label_weather_describe, lv_label_temp, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -50);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            lv_obj_del(lv_label_temp);
            lv_obj_del(lv_label_humidity);
            lv_obj_del(lv_label_local);
            lv_obj_del(lv_label_weather_describe);
        }
    }

    lv_port_sem_give();
}

/**
 * @brief 天气图标事件回调
 *
 * @param obj
 * @param event
 */
static void img_weather_cb(lv_obj_t *obj, lv_event_t event)
{
    uint32_t page_option_data;

    if (event == LV_EVENT_PRESSED)
    {
        page_option_data = PAGE_EXIT;
        xQueueSend(weather_page_queue, &page_option_data, NULL);
    }
}

/**
 * @brief 天气图标移动
 *
 * @param bg_img
 */
static void weather_move(uint8_t sw, void *bg_img)
{
#if WEATHER_MOVE_SW
    static uint32_t init = false;
    static lv_anim_t lv_anim;

    if (sw)
    {
        if (init == false)
        {
            init = true;

            lv_anim_init(&lv_anim);
            lv_anim_set_exec_cb(&lv_anim, (lv_anim_exec_xcb_t)lv_obj_set_x);
            lv_anim_set_var(&lv_anim, bg_img);
            lv_anim_set_time(&lv_anim, 6000);
            lv_anim_set_values(&lv_anim, -150, 320);
            lv_anim_set_repeat_count(&lv_anim, LV_ANIM_REPEAT_INFINITE);
            lv_anim_start(&lv_anim);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            lv_anim_del(bg_img, (lv_anim_exec_xcb_t)lv_obj_set_x);
        }
    }
#endif
}

static void *weather_update_daily_icon(uint32_t num)
{
    static void *_data = NULL;
    char file_path[32];
    lv_fs_file_t file_img;

    size_t file_size = 0;
    size_t real_size = 0;

    switch (num)
    {
    case 0: //晴
        num = 1;
        break;
    case 1: //多云
        num = 2;
        break;
    case 2: //阴
        num = 3;
        break;
    case 3: //阵雨
        num = 9;
        break;
    case 7: //小雨
        num = 10;
        break;
    case 8: //中雨
        num = 11;
        break;
    case 9: //大雨
        num = 12;
        break;
    case 10: //暴雨
        num = 12;
        break;
    case 11: //大暴雨
        num = 12;
        break;
    case 12: //特大暴雨
        num = 12;
        break;
    case 14: //小雪
        num = 14;
        break;
    case 15: //中雪
        num = 15;
        break;
    case 16: //大雪
        num = 16;
        break;
    case 17: //暴雪
        num = 16;
        break;
    default:
        num = 19;
        break;
    }

    /* Try to open file */
    sprintf(file_path, "S:/icon/%u.bin", num);
    if (LV_FS_RES_OK != lv_fs_open(&file_img, file_path, LV_FS_MODE_RD))
    {
        ESP_LOGE(TAG, "Can't open file : %s", file_path);
        return;
    }

    /* Get size of file */
    lv_fs_size(&file_img, &file_size);
    file_size -= 1;

    _data = heap_caps_malloc(file_size + 8, MALLOC_CAP_SPIRAM);
    lv_fs_read(&file_img, _data + 8, file_size, &real_size);
    lv_fs_close(&file_img);

    /* Change lv_img_dsc_t's data pointer */
    memmove(_data, _data + 8, 4);

    /* Set image information */
    *((uint32_t *)(_data + 4)) = file_size - 4;
    *((uint32_t *)(_data + 8)) = (uint32_t)(_data + 12);

    return _data;
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

        page_option_data = PAGE_EXIT;
        xQueueSend(weather_page_queue, &page_option_data, NULL);
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
 * @brief 天气图标显示
 *
 * @param sw
 * @param _data 图标源数据，注意需要释放
 * @return lv_obj_t* 用于动画
 */
lv_obj_t *weather_icon_display(uint8_t sw, void *_data)
{
    static uint8_t init = false;
    static lv_obj_t *img_weather;

    lv_port_sem_take();

    if (sw)
    {
        if (init == false)
        {
            init = true;

            img_weather = lv_img_create(lv_scr_act(), NULL);
            lv_img_set_src(img_weather, _data);
            lv_obj_align(img_weather, NULL, LV_ALIGN_CENTER, -50, -50);
            lv_obj_set_event_cb(img_weather, img_weather_cb);
        }
        else
        {
            lv_img_set_src(img_weather, _data);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            if (_data)
            {
                free(_data);
                _data = NULL;
            }

            lv_obj_del(img_weather);
        }
    }

    lv_port_sem_give();
    return img_weather;
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
        page_option_data = PAGE_EXIT;
        xQueueSend(weather_page_queue, &page_option_data, NULL);
    }
}

/**
 * @brief 退出按钮
 *
 * @param sw 启动 关闭
 */
static void exit_bt_inti(uint8_t sw)
{
    static uint32_t init = false;
    static lv_obj_t *btn_exit = NULL;

    lv_port_sem_take();

    if (sw)
    {
        if (init == false)
        {
            init = true;

            btn_exit = lv_btn_create(lv_scr_act(), NULL);
            lv_obj_set_size(btn_exit, 50, 35);
            lv_obj_set_event_cb(btn_exit, btn_exit_cb);
            lv_obj_align(btn_exit, NULL, LV_ALIGN_IN_TOP_RIGHT, -20, 20);
            lv_obj_set_style_local_value_str(btn_exit, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_HOME);
            lv_obj_set_style_local_bg_color(btn_exit, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_THEME);

            lv_group_remove_all_objs(group);
            lv_group_set_editing(group, true);
            lv_group_add_obj(group, btn_exit);
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            lv_obj_del(btn_exit);
        }
    }

    lv_port_sem_give();
}

/**
 * @brief 天气页面时钟初始化
 *
 * @param sw
 * @param str_time 时间字符串
 * @param str_week 周字符串
 */

/**
 * @brief 天气页面时钟初始化
 *
 * @param sw 启闭
 * @param str_time 时间字符串
 * @param week 周 0~6 = 周日~周六，-1无效
 */
static void weather_time_init(uint8_t sw, char *str_time, int32_t week)
{
    static uint32_t init = false;
    static lv_obj_t *page_humidity = NULL;
    static lv_obj_t *label_time = NULL;
    static lv_obj_t *label_week = NULL;
    const char *week_str[] = {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
    };

    lv_port_sem_take();

    if (sw)
    {
        if (init == false)
        {
            if ((str_time == NULL) && (week == -1))
                return;

            init = true;

            page_humidity = lv_obj_create(lv_scr_act(), NULL);
            lv_obj_set_size(page_humidity, 190, 110);
            lv_obj_set_style_local_bg_color(page_humidity, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_THEME);
            lv_obj_set_style_local_radius(page_humidity, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 10);
            lv_obj_set_style_local_border_width(page_humidity, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
            lv_obj_align(page_humidity, NULL, LV_ALIGN_CENTER, -40, -45);

            label_time = lv_label_create(page_humidity, NULL);
            lv_label_set_text(label_time, str_time);
            lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_bold_60);
            lv_obj_set_style_local_text_color(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
            lv_obj_align(label_time, page_humidity, LV_ALIGN_CENTER, 0, -10);

            label_week = lv_label_create(page_humidity, NULL);
            lv_label_set_text(label_week, week_str[week]);
            lv_obj_set_style_local_text_font(label_week, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &font_en_20);
            lv_obj_set_style_local_text_color(label_week, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
            lv_obj_align(label_week, label_time, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
        }
        else
        {
            if (str_time && (week != -1))
            {
                lv_label_set_text(label_time, str_time);
                lv_label_set_text(label_week, week_str[week]);
            }
        }
    }
    else
    {
        if (init == true)
        {
            init = false;

            lv_obj_del(label_time);
            lv_obj_del(label_week);
            lv_obj_del(page_humidity);
        }
    }
    lv_port_sem_give();
}

/**
 *
 * @brief 页面任务
 *
 */
void weather_page_task()
{
    extern uint32_t wiif_connect_state;
    uint32_t page_option_data;
    static TaskHandle_t TaskHandle = NULL;
    TaskHandle = NULL;

    char strftime_buf[8];
    uint32_t week;

    while (1)
    {
        if (xQueueReceive(weather_page_queue, &page_option_data, 1000 / portTICK_PERIOD_MS))
        {
            switch (page_option_data)
            {
            case PAGE_SHOW:
                if (wiif_connect_state != WIFI_CONNECT_OK)
                {
                    ESP_LOGI(TAG, "未连接WIFI 不能进入天气功能");

                    msgbox_control(true, "No network!");
                    break;
                }
                weather_display = WEATHER_DISPLAY_ICO;
                wallpaper_init(WALLPAPER_FLOWER);
                exit_bt_inti(true);
                xTaskCreate(&http_get_weather, "http_get_weather", 4096, NULL, 5, &TaskHandle);
                break;
            case PAGE_EXIT:
                display_weather(false, NULL);
                exit_bt_inti(false);
                weather_move(false, NULL);
                weather_time_init(false, NULL, -1);
                weather_icon_display(false, NULL);
                msgbox_control(false, NULL);

                if (_data)
                {
                    free(_data);
                    _data = NULL;
                }

                if (TaskHandle)
                {
                    vTaskDelete(TaskHandle);
                    TaskHandle = NULL;
                }

                vTaskDelay(200 / portTICK_RATE_MS);
                page_option_data = PAGE_SHOW;
                xQueueSend(main_page_queue, &page_option_data, NULL);

                cql_page_dele(WEATHER_PAGE_TASK_NUM);
                break;
            case PAGE_LEFT_KEY:
                ESP_LOGE(TAG, "收到键值 左");

                /**
                 * @brief 显示时间
                 *
                 */
                weather_display = WEATHER_DISPLAY_TIME;
                weather_icon_display(false, NULL);

                get_hour_min_week_str(strftime_buf, &week);
                weather_time_init(true, strftime_buf, week);
                break;
            case PAGE_RIGHT_KEY:
                ESP_LOGE(TAG, "收到键值 右");

                /**
                 * @brief 显示图标
                 *
                 */
                if (_data)
                {
                    weather_display = WEATHER_DISPLAY_ICO;
                    weather_time_init(false, NULL, -1);
                    weather_icon_display(true, _data);
                }
                break;
            default:

                break;
            }
        }

        /**
         * @brief 定时更新时间
         *
         */
        if (weather_display == WEATHER_DISPLAY_TIME)
        {
            get_hour_min_week_str(strftime_buf, &week);
            weather_time_init(true, strftime_buf, week);
        }
    }
}

/**
 * @brief 获取时间分钟字符串
 *
 * @param hour_min
 * @param week
 */
void get_hour_min_week_str(char *hour_min, uint32_t *week)
{
    static uint32_t timep = false;
    struct tm *timeinfo = z05d_mb_storage_get_timeinfo();
    char strftime_buf[64];

    strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d %H:%M:%S", timeinfo);

#if 0
    ESP_LOGE(TAG, "当前时间 %s week - %d", strftime_buf, timeinfo->tm_wday);
#endif

    strftime_buf[16] = '\0';
    memcpy(hour_min, &strftime_buf[11], strlen(&strftime_buf[11]));
    if (timep)
    {
        timep = false;
        hour_min[2] = ':';
    }
    else
    {
        timep = true;
        hour_min[2] = ' ';
    }

    *week = timeinfo->tm_wday;
}
