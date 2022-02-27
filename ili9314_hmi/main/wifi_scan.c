/* Scan Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/*
    This example shows how to use the All Channel Scan or Fast Scan to connect
    to a Wi-Fi network.
    In the Fast Scan mode, the scan will stop as soon as the first network matching
    the SSID is found. In this mode, an application can set threshold for the
    authentication mode and the Signal strength. Networks that do not meet the
    threshold requirements will be ignored.
    In the All Channel Scan mode, the scan will end only after all the channels
    are scanned, and connection will start with the best network. The networks
    can be sorted based on Authentication Mode or Signal Strength. The priority
    for the Authentication mode is:  WPA2 > WPA > WEP > Open
*/
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "string.h"
#include "stdio.h"
#include "wifi_scan.h"
#include "wifi_scan_page.h"
#include "main.h"

#define TAG "WIFI_SCAN"

#if CONFIG_WIFI_ALL_CHANNEL_SCAN
#define DEFAULT_SCAN_METHOD WIFI_ALL_CHANNEL_SCAN
#elif CONFIG_WIFI_FAST_SCAN
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#else
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#endif /*CONFIG_SCAN_METHOD*/

#if CONFIG_WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#elif CONFIG_WIFI_CONNECT_AP_BY_SECURITY
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SECURITY
#else
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#endif /*CONFIG_SORT_METHOD*/

#if CONFIG_FAST_SCAN_THRESHOLD
#define DEFAULT_RSSI CONFIG_FAST_SCAN_MINIMUM_SIGNAL
#if CONFIG_EXAMPLE_OPEN
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#elif CONFIG_EXAMPLE_WEP
#define DEFAULT_AUTHMODE WIFI_AUTH_WEP
#elif CONFIG_EXAMPLE_WPA
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA_PSK
#elif CONFIG_EXAMPLE_WPA2
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA2_PSK
#else
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif
#else
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif /*CONFIG_FAST_SCAN_THRESHOLD*/

wifi_ap_record_t *wifi_ap_record = NULL;
uint16_t wifi_ap_record_num = 0;
static char *ipstring[32] = {0};
EventGroupHandle_t wifi_event_group;
static uint8_t connect_num = 0;
static int init = false;
static wifi_scan_config_t scanConf = {
    .ssid = NULL,
    .bssid = NULL,
    .channel = 0,
    .show_hidden = 0,
};

extern xQueueHandle main_page_queue;
uint32_t wifi_op_typed = WIFI_OP_TYPED_NULL;

/**
 * @brief Get the ip str object 输出ipv4地址字符串
 * 
 * @return char* 字符串指针地址
 */
char *get_ip_str()
{
    if (strlen(ipstring) != 0)
    {
        return ipstring;
    }
    else
    {
        return NULL;
    }
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    uint32_t page_option_data;

    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
        ESP_LOGI(TAG, "Got IP: %s\n",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));

        memcpy(ipstring, ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip), strlen(ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip)));
        xEventGroupSetBits(wifi_event_group, CONNECT_DONE_BIT);

        wifi_op_typed = WIFI_OP_TYPED_NULL;

        page_option_data = PAGE_HANDLE_1;
        xQueueSend(main_page_queue, &page_option_data, NULL);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");

        page_option_data = PAGE_HANDLE_2;
        xQueueSend(main_page_queue, &page_option_data, NULL);

        if (wifi_op_typed == WIFI_OP_TYPED_TRY_CONNECT)
        {
            connect_num++;
            if (connect_num < 3)
            {
                ESP_ERROR_CHECK(esp_wifi_connect());
            }
            else
            {
                xEventGroupSetBits(wifi_event_group, CONNECT_FAIL_BIT);
                wifi_op_typed = WIFI_OP_TYPED_NULL;
            }
        }

        if (wifi_op_typed == WIFI_OP_TYPED_CONNECT)
        {
            connect_num++;
            if (connect_num < 100)
            {
                ESP_ERROR_CHECK(esp_wifi_connect());
            }
            else
            {
                xEventGroupSetBits(wifi_event_group, CONNECT_FAIL_BIT);
                wifi_op_typed = WIFI_OP_TYPED_NULL;
            }
        }
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        xEventGroupSetBits(wifi_event_group, SCAN_DONE_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void scan_task(void *pvParameters)
{
    uint32_t page_option_data;
    EventBits_t EventBits;
    while (1)
    {
        EventBits = xEventGroupWaitBits(wifi_event_group, SCAN_DONE_BIT | CONNECT_WIFI_BIT | TRY_CONNECT_WIFI_BIT, true, false, portMAX_DELAY);
        xEventGroupClearBits(wifi_event_group, EventBits);

        if (EventBits & SCAN_DONE_BIT)
        {
            wifi_op_typed = WIFI_OP_TYPED_SCAN_DONE;
            ESP_LOGI(TAG, "WIFI扫描完毕");
            esp_wifi_scan_get_ap_num(&wifi_ap_record_num);
            printf("Number of access points found: %d\n", wifi_ap_record_num);
            if (!wifi_ap_record_num)
            {
                ESP_LOGI(TAG, "Nothing AP found");
                return;
            }

            wifi_ap_record = heap_caps_malloc(sizeof(wifi_ap_record_t) * wifi_ap_record_num, MALLOC_CAP_SPIRAM);

            ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&wifi_ap_record_num, wifi_ap_record));
            for (int i = 0; i < wifi_ap_record_num; i++)
            {
                ESP_LOGI(TAG, "(%d,\"%s\",%d,\"" MACSTR " %d\")\r\n", wifi_ap_record[i].authmode, wifi_ap_record[i].ssid, wifi_ap_record[i].rssi,
                         MAC2STR(wifi_ap_record[i].bssid), wifi_ap_record[i].primary);
            }

            xEventGroupSetBits(wifi_event_group, SCAN_LIST_DONE_BIT);
        }

        if (EventBits & CONNECT_WIFI_BIT)
        {
            wifi_op_typed = WIFI_OP_TYPED_CONNECT;
            connect_num = 0;
            wifi_config_t wifi_config = {0};
            memset(&wifi_config, 0, sizeof(wifi_config));
            get_wifi_scan_page_ssid_info((char *)wifi_config.sta.ssid, (char *)wifi_config.sta.password);
            ESP_LOGI(TAG, "收到请求连接WIFI, SSID: %s, PWD: %s", wifi_config.sta.ssid, wifi_config.sta.password);

            ESP_ERROR_CHECK(esp_wifi_disconnect());
            vTaskDelay(1000 / portTICK_RATE_MS);
            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
            ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
            ESP_ERROR_CHECK(esp_wifi_connect());

            page_option_data = PAGE_HANDLE_2;
            xQueueSend(main_page_queue, &page_option_data, NULL);
        }

        if (EventBits & TRY_CONNECT_WIFI_BIT)
        {
            wifi_op_typed = WIFI_OP_TYPED_TRY_CONNECT;
            connect_num = 0;
            wifi_config_t wifi_config = {0};
            memset(&wifi_config, 0, sizeof(wifi_config));
            get_wifi_scan_page_ssid_info((char *)wifi_config.sta.ssid, (char *)wifi_config.sta.password);
            ESP_LOGI(TAG, "收到尝试请求连接WIFI, SSID: %s, PWD: %s", wifi_config.sta.ssid, wifi_config.sta.password);

            ESP_ERROR_CHECK(esp_wifi_disconnect());
            vTaskDelay(1000 / portTICK_RATE_MS);
            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
            ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
            ESP_ERROR_CHECK(esp_wifi_connect());

            page_option_data = PAGE_HANDLE_2;
            xQueueSend(main_page_queue, &page_option_data, NULL);
        }
    }
}

void wifi_scan_init(void)
{
    uint32_t page_option_data;

    if (init == false)
    {
        init = true;
        wifi_op_typed = WIFI_OP_TYPED_SCAN_START;
        ESP_LOGI(TAG, "扫描WIFI");
        tcpip_adapter_init();
        wifi_event_group = xEventGroupCreate();                    //创建一个事件标志组
        ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL)); //创建事件的任务
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();       //设置默认的wifi栈参数
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));                      //初始化WiFi Alloc资源为WiFi驱动，如WiFi控制结构，RX / TX缓冲区，WiFi NVS结构等，此WiFi也启动WiFi任务。
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));   // Set the WiFi API configuration storage type
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));         //Set the WiFi operating mode
        ESP_ERROR_CHECK(esp_wifi_start());
        xTaskCreate(&scan_task, "scan_task", 2048, NULL, 15, NULL); //创建扫描任务
        ESP_ERROR_CHECK(esp_wifi_scan_start(&scanConf, 1));         //The true parameter cause the function to block
    }
    else
    {
        wifi_op_typed = WIFI_OP_TYPED_SCAN_START;
        ESP_ERROR_CHECK(esp_wifi_disconnect());
        vTaskDelay(1000 / portTICK_RATE_MS);
        ESP_ERROR_CHECK(esp_wifi_scan_start(&scanConf, 1)); //The true parameter cause the function to block
    }

    page_option_data = PAGE_HANDLE_2;
    xQueueSend(main_page_queue, &page_option_data, NULL);
}

void wifi_connect_init(uint8_t ssid[32], uint8_t password[64])
{
    wifi_config_t wifi_config = {0};

    if (init == false)
    {
        init = true;
        wifi_op_typed = WIFI_OP_TYPED_SCAN_START;
        ESP_LOGI(TAG, "扫描WIFI");
        tcpip_adapter_init();
        wifi_event_group = xEventGroupCreate();                    //创建一个事件标志组
        ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL)); //创建事件的任务
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();       //设置默认的wifi栈参数
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));                      //初始化WiFi Alloc资源为WiFi驱动，如WiFi控制结构，RX / TX缓冲区，WiFi NVS结构等，此WiFi也启动WiFi任务。
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));   // Set the WiFi API configuration storage type
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));         //Set the WiFi operating mode
        ESP_ERROR_CHECK(esp_wifi_start());
        xTaskCreate(&scan_task, "scan_task", 2048, NULL, 15, NULL); //创建扫描任务

        memset(&wifi_config, 0, sizeof(wifi_config));
        memcpy(&wifi_config.ap.ssid, ssid, strlen((char *)ssid));
        memcpy(&wifi_config.ap.password, password, strlen((char *)password));

        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_connect());
    }
    else
    {
        memset(&wifi_config, 0, sizeof(wifi_config));
        memcpy(&wifi_config.ap.ssid, ssid, strlen((char *)ssid));
        memcpy(&wifi_config.ap.password, password, strlen((char *)password));

        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_connect());
    }
}