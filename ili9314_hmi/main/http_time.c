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
#include "sys/time.h"

#define TAG "HTTP_TIME"

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "api.m.taobao.com"
#define WEB_PORT "80"
#define WEB_PATH "/rest/api3.do?api=mtop.common.getTimestamp"

#define WEATHER_TEXT_COLOR LV_COLOR_WHITE

static const char *REQUEST = "GET " WEB_PATH " HTTP/1.0\r\n"
                             "Host: " WEB_SERVER ":" WEB_PORT "\r\n"
                             "User-Agent: esp-idf/1.0 esp32\r\n"
                             "\r\n";

static RTC_DATA_ATTR struct timeval sleep_enter_time;

void http_get_time(void *pvParameters)
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
                            json_key2 = cJSON_GetObjectItem(json_key1, "t");

                            if (json_key2)
                            {
                                /**
                                 * @brief 截去ms部分
                                 *
                                 */
                                char sec_str[32];
                                sprintf(sec_str, "%s", cJSON_GetStringValue(json_key2));
                                sec_str[10] = '\0';

                                /**
                                 * @brief 字符串转数值
                                 *
                                 */
                                long timesmap = atoi(sec_str);
                                ESP_LOGE(TAG, "获取到时间戳 %ld\r\n", timesmap);

                                /**
                                 * @brief 设置系统时间
                                 *
                                 */
                                sleep_enter_time.tv_sec = timesmap;
                                sleep_enter_time.tv_usec = 0;
                                settimeofday(&sleep_enter_time, NULL);

                                /**
                                 * @brief 重新读取时间
                                 *
                                 */
                                struct tm *timeinfo = z05d_mb_storage_get_timeinfo();
                                char strftime_buf[64];

                                strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
                                ESP_LOGE(TAG, "设置时间 当前时间 %s", strftime_buf);
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

struct tm *z05d_mb_storage_get_timeinfo(void)
{
    time_t now;
    static struct tm timeinfo;

    time(&now);
    setenv("TZ", "CST-8", 1);
    tzset();

    localtime_r(&now, &timeinfo);
    return &timeinfo;
}

void z05d_mb_storage_set_time(struct tm *timeinfo)
{
    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
    time_t t = mktime(timeinfo);
    sleep_enter_time.tv_sec = t;
    sleep_enter_time.tv_usec = 0;
    settimeofday(&sleep_enter_time, NULL);
}
