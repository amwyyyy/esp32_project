/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include "u8g2.h"
#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

void task_test_SSD1306i2c(void *ignore) {
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda = 21;
	u8g2_esp32_hal.scl = 22;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

    u8g2_t u8g2;
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
		&u8g2,
		U8G2_R0,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure
	u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_lubB18_tr);
    u8g2_DrawStr(&u8g2, 10, 40, "22:44");
    u8g2_SendBuffer(&u8g2);

    for (int i = 0; i < 100; i++) {
        u8g2_DrawBox(&u8g2, 10, 26, i, 6);
        u8g2_DrawFrame(&u8g2, 10, 26, 100, 6);
        u8g2_SendBuffer(&u8g2);
        printf("current core: %d\n", xPortGetCoreID());
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

void app_main(void)
{
    printf("Hello world!\n");

    xTaskCreatePinnedToCore(task_test_SSD1306i2c, "test_task", 1024 * 4, NULL, 2, NULL, tskNO_AFFINITY);

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Size of available internal heap: %d bytes\n", esp_get_free_internal_heap_size());

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    printf("task nums: %d\n", uxTaskGetNumberOfTasks());

    // char InfoBuffer[512];
    // for (int i = 10; i >= 0; i--) {
    //     vTaskList((char *) &InfoBuffer);
    //     printf("任务名   任务状态    优先级  剩余栈  任务序号    CPU核\n");
    //     printf("\n%s\n", InfoBuffer);
        
    //     vTaskGetRunTimeStats((char *) &InfoBuffer);
    //     printf("任务名       运行计数         使用率");
    //     printf("\n%s\n", InfoBuffer);

    //     vTaskDelay(10000 / portTICK_PERIOD_MS);
    // }
    // printf("Restarting now.\n");
    // fflush(stdout);
    // esp_restart();
}
