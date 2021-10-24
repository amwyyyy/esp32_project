#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_sntp.h"
#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

#define PIN_SDA 21
#define PIN_SCL 22

#define BLINK_GPIO 2
#define HIGH 1
#define LOW  0

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "clock_idf";

/* FreeRTOS event group to signal when we are connected */
static EventGroupHandle_t s_wifi_event_group;

u8g2_t u8g2;

char wifi_ssid[32];
char wifi_passwd[64];

bool wifi_connected = false;
bool sntp_config    = false;

struct {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} now_time;

// 方法定义
void initialize_sntp(void *arg);
void get_now_time(void);
void task_draw_local_time(void *arg);
void get_by_nvs();
void light_led(void);

void u8g2_init() {
    ESP_LOGI(TAG, "Init u8g2...");
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda = PIN_SDA;
	u8g2_esp32_hal.scl = PIN_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

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

        light_led();
        wifi_connected = true;
        xTaskCreatePinnedToCore(initialize_sntp, "InitializeSntp", 1024 * 2, NULL, 1, NULL, tskNO_AFFINITY);
    }
}

/**
 * 初始化网络 
 */
void wifi_init(void *arg) {
    ESP_LOGI(TAG, "Init WiFi...");
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
    ESP_LOGI(TAG, "WiFi ssid: %s\tpasswd: %s", wifi_ssid, wifi_passwd);

    // 设置 WiFi 运行模式为: STA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    // 设置 WiFi 连接参数
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    // 启动 WiFi
    ESP_ERROR_CHECK(esp_wifi_start());
    // 设置 WiFi 省电模式
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_MAX_MODEM));

    ESP_LOGI(TAG, "Init WiFi end.");

    vTaskDelete(NULL);
}

/**
 * 从 nvs 获取 wifi 账号密码
 */
void get_by_nvs() {
    ESP_LOGI(TAG, "Init NVS...");

    // 初始化存储
    ESP_ERROR_CHECK(nvs_flash_init());

    // 定义NVS操作句柄
    nvs_handle wificfg_nvs_handler;

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

    ESP_LOGI(TAG, "Close NVS.");
}

/**
 * 初始化Sntp
 */
void initialize_sntp(void *arg) {
	ESP_LOGI(TAG, "------------Initializing SNTP-----------");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);  
	sntp_setservername(0, "ntp.aliyun.com"); 
    sntp_setservername(1, "ntp1.aliyun.com"); 
	sntp_init();

    // 设置时区
    setenv("TZ", "CST-8", 1);
	tzset();

    time_t now = 0;
    struct tm timeinfo = {0};

    do {
        time(&now);
        localtime_r(&now, &timeinfo);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    } while (timeinfo.tm_year < 100);

    ESP_LOGI(TAG, "------------Finish SNTP-----------");
    sntp_config = true;
    vTaskDelete(NULL);
}

void get_now_time(void) {
    time_t now = 0;
    struct tm timeinfo = {0};

    time(&now);
    localtime_r(&now, &timeinfo);

    now_time.year   = timeinfo.tm_year + 1900;
    now_time.month  = timeinfo.tm_mon + 1;
    now_time.day    = timeinfo.tm_mday;
    now_time.hour   = timeinfo.tm_hour;
    now_time.minute = timeinfo.tm_min;
    now_time.second = timeinfo.tm_sec;

    ESP_LOGD(TAG, "-------current time: %d-%d-%d %d:%d:%d",
			 now_time.year, now_time.month,
			 now_time.day, now_time.hour,
			 now_time.minute, now_time.second);        
}

void task_draw_local_time(void *arg) {
    // 保存格式化字符串
    char *ld     = (char *) malloc(11);
    char *lt     = (char *) malloc(6);
    char *second = (char *) malloc(3);
    
    while (true) {
        get_now_time();
        u8g2_FirstPage(&u8g2);
        do {
            // 画网格线
            u8g2_DrawVLine(&u8g2, 45, 0, 18);
            u8g2_DrawVLine(&u8g2, 70, 0, 18);
            u8g2_DrawHLine(&u8g2, 0, 18, 128);
            u8g2_DrawHLine(&u8g2, 0, 51, 128);
            // 显示时分
            u8g2_SetFont(&u8g2, u8g2_font_lubB18_tr);
            sprintf(lt, "%02d:%02d", now_time.hour, now_time.minute);
            u8g2_DrawStr(&u8g2, 22, 45, lt);
            // 显示秒
            u8g2_SetFont(&u8g2, u8g2_font_8x13B_tr);
            sprintf(second, "%02d", now_time.second);
            u8g2_DrawStr(&u8g2, 98, 45, second);
            // 显示年月日
            u8g2_SetFont(&u8g2, u8g2_font_profont11_tr);
            sprintf(ld, "%d.%02d.%02d", now_time.year, now_time.month, now_time.day);
            u8g2_DrawStr(&u8g2, 35, 60, ld);

            vTaskDelay(200 / portTICK_PERIOD_MS);
        } while (u8g2_NextPage(&u8g2));
    }
}

void display_process(void *arg) {
    char current_process[] = "Connecting WiFi...";
    int i = 1;
    for (; i <= 100; i++) {
        if (wifi_connected) {
            strcpy(current_process, "Config Sntp...");
        }

        if (sntp_config) {
            if (i < 100) {
                for (; i <= 100; i++) {
                    u8g2_FirstPage(&u8g2);
                    do {
                        u8g2_DrawBox(&u8g2, 10, 30, i, 6);
                        u8g2_DrawFrame(&u8g2, 10, 30, 100, 6);
                        u8g2_SetFont(&u8g2, u8g2_font_profont11_tr);
                        u8g2_DrawStr(&u8g2, 10, 50, current_process);
                    } while (u8g2_NextPage(&u8g2));
                }
            }
            break;
        }

        u8g2_FirstPage(&u8g2);
        do {
            u8g2_DrawBox(&u8g2, 10, 30, i, 6);
            u8g2_DrawFrame(&u8g2, 10, 30, 100, 6);
            u8g2_SetFont(&u8g2, u8g2_font_profont11_tr);
            u8g2_DrawStr(&u8g2, 10, 50, current_process);
        } while (u8g2_NextPage(&u8g2));
        vTaskDelay(200 / portTICK_PERIOD_MS);
        if (i == 100) {
            i = 0;
        }
    }

    xTaskCreatePinnedToCore(task_draw_local_time, "DrawLocalTime", 1024 * 4, NULL, 2, NULL, 1);
    vTaskDelete(NULL);
}

void light_led(void) {
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(BLINK_GPIO, HIGH);    
}

void app_main(void) {
    ESP_LOGI(TAG, "APP is start!!!");

    u8g2_init();

    get_by_nvs();

    xTaskCreatePinnedToCore(wifi_init, "WiFiInit", 1024 * 4, NULL, 1, NULL, tskNO_AFFINITY);

    xTaskCreatePinnedToCore(display_process, "DisplayProcess", 1024 * 2, NULL, 2, NULL, tskNO_AFFINITY);
}
