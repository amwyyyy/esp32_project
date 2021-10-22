#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

#define PIN_SDA 21
#define PIN_SCL 22

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "clock_idf";

/* FreeRTOS event group to signal when we are connected */
static EventGroupHandle_t s_wifi_event_group;

// void task_test_SSD1306i2c(void *ignore) {
//     u8g2_FirstPage(&u8g2);
//     do {
//         u8g2_SetFont(&u8g2, u8g2_font_crox4hb_tf);
//         u8g2_DrawStr(&u8g2, 10, 50, "22:44");
//     } while (u8g2_NextPage(&u8g2));

//     vTaskDelete(NULL);
// }

void u8g2_init() {
    ESP_LOGI(TAG, "Init u8g2...");
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda = PIN_SDA;
	u8g2_esp32_hal.scl = PIN_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

    u8g2_t u8g2;
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
		&u8g2,
		U8G2_R0,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);
	u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearDisplay(&u8g2);
    ESP_LOGI(TAG, "Init u8g2 end.");
}

/**
 * WiFi 事件处理
 */
static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data) {
    static int s_retry_num = 0;
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < 5) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

/**
 * 初始化网络 
 */
void wifi_init(char *wifi_ssid, char *wifi_passwd) {
    ESP_ERROR_CHECK(esp_netif_init());  // 初始化网络接口

    s_wifi_event_group = xEventGroupCreate();   // 创建WIFI连接事件组
    ESP_ERROR_CHECK(esp_event_loop_create_default());   // 默认事件循环

    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 注册 WiFi 事件
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    wifi_config_t wifi_config;
    bzero(&wifi_config, sizeof(wifi_config_t));
    memcpy(wifi_config.sta.ssid, wifi_ssid, sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.sta.password, wifi_passwd, sizeof(wifi_config.sta.password));

    // 设置 WiFi 运行模式为: STA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    // 设置 WiFi 连接参数
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    // 启动 WiFi
    ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main(void) {
    ESP_LOGI(TAG, "\nAPP is start!!!\n");
    u8g2_init();

    // 初始化存储
    ESP_ERROR_CHECK(nvs_flash_init());

    // 定义NVS操作句柄
    nvs_handle wificfg_nvs_handler;
    char *wifi_ssid = malloc(32);
    char *wifi_passwd = malloc(64);
    size_t len;

    // 打开NVS命名空间
    ESP_ERROR_CHECK(nvs_open("esp32-clock", NVS_READWRITE, &wificfg_nvs_handler));

    // 获取值
    len = sizeof(wifi_ssid);
    ESP_ERROR_CHECK(nvs_get_str(wificfg_nvs_handler, "wifi_ssid", wifi_ssid, &len));
    len = sizeof(wifi_ssid);
    ESP_ERROR_CHECK(nvs_get_str(wificfg_nvs_handler, "wifi_passwd", wifi_passwd, &len));

    // 提交
    ESP_ERROR_CHECK(nvs_commit(wificfg_nvs_handler));

    // 关闭
    nvs_close(wificfg_nvs_handler);

    wifi_init(wifi_ssid, wifi_passwd); 
}
