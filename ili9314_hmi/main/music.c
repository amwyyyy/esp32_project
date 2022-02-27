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
#include "storage.h"

#define TAG "spiffs"

esp_err_t spiffs_init(void)
{
    esp_err_t ret = ESP_OK;
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    /*!< Use settings defined above to initialize and mount SPIFFS filesystem. */
    /*!< Note: esp_vfs_spiffs_register is an all-in-one convenience function. */
    ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }

        return ret;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    /*!< Open renamed file for reading */
    ESP_LOGI(TAG, "Reading file");
    FILE *f = fopen("/spiffs/spiffs.txt", "r");

    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }

    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    /*!< strip newline */
    char *pos = strchr(line, '\n');

    if (pos)
    {
        *pos = '\0';
    }

    ESP_LOGI(TAG, "Read from file: '%s'", line);

    return ESP_OK;
}

void music_init()
{
    // ESP_ERROR_CHECK(spiffs_init());
    ESP_ERROR_CHECK(i2c_bus2_init());

    extern xQueueHandle play_queue;
    play_queue = xQueueCreate(1, sizeof(uint32_t));

    uint32_t vol = 60;
    if (get_setting_vol(&vol) != ESP_OK)
    {
        ESP_LOGI(TAG, "设置默认音量值(%d)", vol);
        set_setting_vol(vol);
    }

    audio_init(vol * 10);
}
