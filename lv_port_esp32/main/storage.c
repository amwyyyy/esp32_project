#include <stdio.h>
#include "sdkconfig.h"

#include "esp_heap_caps.h"
#include "esp_spi_flash.h"
#include "esp_system.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_wifi_types.h"
#include "storage.h"

static nvs_handle_t storage_tv_handle;

static const char *TAG = "storage";

esp_err_t get_wifi_info(char *ssid, char *passwd) {
    esp_err_t result = 0;
    char ssid_tmp[32] = {0};
    char passwd_tmp[64] = {0};
    
    size_t len = sizeof(ssid_tmp);
    result = nvs_get_blob(storage_tv_handle, "_ssid", ssid_tmp, &len);
    ESP_LOGI(TAG, "SSID=%.*s;len=%d;", len, ssid_tmp, len);
    if (result != ESP_OK) {
        return result;
    } else {
        for (size_t i = 0; i < len; i++) {
            ssid[i] = ssid_tmp[i];
        }
    }        

    len = sizeof(passwd_tmp);
    result = nvs_get_blob(storage_tv_handle, "_passwd", passwd_tmp, &len);
    ESP_LOGI(TAG, "PASSWD=%.*s;len=%d;", len, passwd_tmp, len);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "This auth is not WIFI_AUTH_OPEN!");
        ESP_LOGE(TAG, "But the password acquisition failed!");
        return result;
    } else {
        for (size_t i = 0; i < len; i++) {
            passwd[i] = passwd_tmp[i];
        }
    }

    return ESP_OK;
}

esp_err_t set_wifi_info(char ssid[32], char passwd[64]) {
    ESP_LOGI(TAG, "设置WIFI信息到 NVS");
    esp_err_t result = 0;
    if (ssid != NULL) {
        ESP_LOGI(TAG, "set ssid: %s", ssid);
        ESP_ERROR_CHECK(nvs_set_blob(storage_tv_handle, "_ssid", ssid, 32));
    }
    
    if (passwd != NULL) {
        ESP_LOGI(TAG, "set passwd: %s", passwd);
        ESP_ERROR_CHECK(nvs_set_blob(storage_tv_handle, "_passwd", passwd, 64));
    }

    return result;
}

esp_err_t get_setting_bl(uint32_t *value) {
    esp_err_t err = nvs_get_u32(storage_tv_handle, "_bl", value);
    switch (err) {
        case ESP_OK:
            ESP_LOGI(TAG, "读取背光信息(%d)", *value);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGW(TAG, "背光配置未初始化!");
            break;
        default :
            ESP_LOGE(TAG, "Error (%s) reading!\n", esp_err_to_name(err));
    }
    return err;
}

esp_err_t set_setting_bl(uint32_t value) {
    ESP_LOGI(TAG, "设置背光信息(%d)", value);

    esp_err_t err = nvs_set_u32(storage_tv_handle, "_bl", value);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "set bl failed! Error (%s)", esp_err_to_name(err));
    }
    return err;
}

esp_err_t storage_init(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    err = nvs_open("storage_tv", NVS_READWRITE, &storage_tv_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}