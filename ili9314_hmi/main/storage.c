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
#include "esp_wifi_types.h"
#include "storage.h"

static nvs_handle storage_tv_handle;

static const char *TAG = "storage_tv";

esp_err_t get_wifi_info(uint8_t *ssid, uint8_t *passwd)
{
    esp_err_t result = 0;
    uint8_t ssid_tmp[32] = {0};
    uint8_t passwd_tmp[64] = {0};
    /*>! 读取SSID */
    size_t len = sizeof(ssid_tmp);
    result = nvs_get_blob(storage_tv_handle, "_ssid", ssid_tmp, &len);
    ESP_LOGI(TAG, "SSID=%.*s;len=%d;", len, ssid_tmp, len);
    if (result != ESP_OK)
        return result;
    else
        for (size_t i = 0; i < len; i++)
            ssid[i] = ssid_tmp[i];

    len = sizeof(passwd_tmp);
    result = nvs_get_blob(storage_tv_handle, "_passwd", passwd_tmp, &len);
    ESP_LOGI(TAG, "PASSWD=%.*s;len=%d;", len, passwd_tmp, len);
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "This auth is not WIFI_AUTH_OPEN!");
        ESP_LOGE(TAG, "But the password acquisition failed!");
        return result;
    }
    else
        for (size_t i = 0; i < len; i++)
            passwd[i] = passwd_tmp[i];

    return ESP_OK;
}

esp_err_t set_wifi_info(char ssid[32], char passwd[64])
{
    ESP_LOGI(TAG, "设置WIFI信息到NVS");
    esp_err_t result = 0;
    if (ssid != NULL)
    {
        ESP_LOGI(TAG, "set ssid");
        ESP_ERROR_CHECK(nvs_set_blob(storage_tv_handle, "_ssid", ssid, 32));
    }
    if (passwd != NULL)
    {
        ESP_LOGI(TAG, "set passwd");
        ESP_ERROR_CHECK(nvs_set_blob(storage_tv_handle, "_passwd", passwd, 64));
    }

    return result;
}

esp_err_t get_setting_vol(uint32_t *value)
{
    esp_err_t result = 0;
    ESP_LOGI(TAG, "读取音量信息");

    result = nvs_get_u32(storage_tv_handle, "_vol", value);
    return result;
}

esp_err_t set_setting_vol(uint32_t value)
{
    esp_err_t result = 0;
    ESP_LOGI(TAG, "设置音量信息(%d)", value);

    ESP_ERROR_CHECK(nvs_set_u32(storage_tv_handle, "_vol", value));
    return result;
}

esp_err_t get_setting_bl(uint32_t *value)
{
    esp_err_t result = 0;
    ESP_LOGI(TAG, "读取背光信息");

    result = nvs_get_u32(storage_tv_handle, "_bl", value);
    return result;
}

esp_err_t set_setting_bl(uint32_t value)
{
    esp_err_t result = 0;
    ESP_LOGI(TAG, "设置背光信息(%d)", value);

    ESP_ERROR_CHECK(nvs_set_u32(storage_tv_handle, "_bl", value));
    return result;
}

esp_err_t storage_init(void)
{
    ESP_ERROR_CHECK(nvs_open("storage_tv", NVS_READWRITE, &storage_tv_handle));
    return ESP_OK;
}
