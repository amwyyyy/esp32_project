/* MQTT over SSL Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "TUYA_LINK_EXAMPLE";

#include "tuya_log.h"
#include "tuya_config.h"
#include "tuya_iot.h"
#include "cJSON.h"
#include "qrcode.h"

/* Tuya device handle */
tuya_iot_client_t client;

#define SWITCH_DP_ID_KEY "1"

#define HUMAN_GPIO 10

int old_status = 0;
int human_status = 0;
int light = 0;

void example_qrcode_print(const char* productkey, const char* uuid)
{
	TY_LOGI("https://smartapp.tuya.com/s/p?p=%s&uuid=%s&v=2.0", productkey, uuid);

    char urlbuf[255];
	sprintf(urlbuf, "https://smartapp.tuya.com/s/p?p=%s&uuid=%s&v=2.0", productkey, uuid);
    qrcode_display(urlbuf);

	TY_LOGI("(Use this URL to generate a static QR code for the Tuya APP scan code binding)");
}

/* DP data reception processing function */
void tuya_iot_dp_download(tuya_iot_client_t* client, const char* json_dps)
{
    TY_LOGD("Data point download value:%s", json_dps);
}

/* Tuya SDK event callback */
static void user_event_handler_on(tuya_iot_client_t* client, tuya_event_msg_t* event)
{
    switch(event->id){
    case TUYA_EVENT_BIND_START:
        example_qrcode_print(client->config.productkey, client->config.uuid);
        break;

    case TUYA_EVENT_MQTT_CONNECTED:
        TY_LOGI("Device MQTT Connected!");
        break;

    case TUYA_EVENT_DP_RECEIVE:
        tuya_iot_dp_download(client, (const char*)event->value.asString);
        break;

    default:
        break;
    }
}

static void status_report_task(void *pvParameters) {
    for(;;) {
        if (human_status != old_status) {
            if (human_status == 1) {
                if (light > 400) {
                    const char bool_value[] = {"{\"101\":true}"};
                    tuya_iot_dp_report_json(&client, bool_value);
                }
            } else {
                if (light < 300) {
                    const char bool_value[] = {"{\"101\":false}"};
                    tuya_iot_dp_report_json(&client, bool_value);
                }
            }
            old_status = human_status;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void adc1task(void* arg)
{
     adc1_config_width(ADC_WIDTH_BIT_DEFAULT); // 设定捕获宽度
     adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // 设定捕获通道
     while(1) {
         light = adc1_get_raw(ADC1_CHANNEL_0);
         printf("The adc1 value: %d\n", light); // 读取在单个通道上的ADC读数
         vTaskDelay(pdMS_TO_TICKS(2000));
     }
}

static void tuya_link_app_task(void *pvParameters)
{
    int ret = OPRT_OK;

    /* Initialize Tuya device configuration */
    ret = tuya_iot_init(&client, &(const tuya_iot_config_t){
        .software_ver = "1.0.0",
        .productkey = TUYA_PRODUCT_KEY,
        .uuid = TUYA_DEVICE_UUID,
        .authkey = TUYA_DEVICE_AUTHKEY,
        .storage_namespace = "tuya_kv",
        .event_handler = user_event_handler_on
    });

    assert(ret == OPRT_OK);

    /* Start tuya iot task */
    tuya_iot_start(&client);

    xTaskCreate(status_report_task, "status_report", 1024 * 6, NULL, 4, NULL);

    for(;;) {
        /* Loop to receive packets, and handles client keepalive */
        tuya_iot_yield(&client);
    }
}

void read_hc_sr501() {
    gpio_reset_pin(HUMAN_GPIO);  // 重置GPIO
    gpio_set_direction(HUMAN_GPIO, GPIO_MODE_INPUT); // 将GPIO设置为输入模式

    gpio_reset_pin(19);
    gpio_set_direction(19, GPIO_MODE_OUTPUT);

    while (1) {
        // 根据GPIO的高低电平判断是否感应到人体活动
        human_status = gpio_get_level(HUMAN_GPIO);
        if (human_status == 1) {
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            human_status = gpio_get_level(HUMAN_GPIO);
        }

        ESP_LOGI(TAG, "是否人体活动 %s!", human_status == 0 ? "无" : "有");
        gpio_set_level(19, human_status);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    xTaskCreate(tuya_link_app_task, "tuya_link", 1024 * 6, NULL, 4, NULL);
    xTaskCreate(read_hc_sr501, "read_hc_sr501", 1024 * 6, NULL, 4, NULL);
    xTaskCreate(adc1task, "adc1task", 1024 * 6, NULL, 4, NULL);
}
