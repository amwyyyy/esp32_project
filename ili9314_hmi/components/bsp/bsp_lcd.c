/**
 * @file bsp_lcd.c
 * @brief LCD initialize and basic operator.
 * @version 0.1
 * @date 2021-01-14
 * 
 * @copyright Copyright 2021 Espressif Systems (Shanghai) Co. Ltd.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *               http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "bsp_lcd.h"

static const char *TAG = "bsp_lcd";

static scr_driver_t lcd;
static scr_info_t lcd_info;

esp_err_t bsp_lcd_init(void)
{
    scr_controller_config_t lcd_cfg = {0};
    scr_interface_driver_t *iface_drv_spi;
    spi_config_t spi_cfg = {
        .miso_io_num = 39,
        .mosi_io_num = 9,
        .sclk_io_num = 8,
        .max_transfer_sz = 240 * 380,
    };
    spi_bus_handle_t spi_bus = spi_bus_create(2, &spi_cfg);
    scr_interface_spi_config_t scr_interface_spi_config = {
        .clk_freq = 10000000,
        .spi_bus = spi_bus,
        .swap_data = false,
        .pin_num_cs = 12,
        .pin_num_dc = 11,
    };
    scr_interface_create(SCREEN_IFACE_SPI, &scr_interface_spi_config, &iface_drv_spi);

    lcd_cfg.interface_drv = iface_drv_spi,
    lcd_cfg.pin_num_rst = 10,
    lcd_cfg.pin_num_bckl = -1,//13
    lcd_cfg.rst_active_level = 0,
    lcd_cfg.bckl_active_level = 1,
    lcd_cfg.width = 240;
    lcd_cfg.height = 320;
    lcd_cfg.rotate = SCR_DIR_TBRL;
    scr_find_driver(SCREEN_CONTROLLER_ST7789, &lcd);
    lcd.init(&lcd_cfg);

    /* Print LCD infomation */
    lcd.get_info(&lcd_info);
    ESP_LOGI(TAG, "Screen name : [%s] | width : [%d] | height : [%d]", lcd_info.name, lcd_info.width, lcd_info.height);

    return ESP_OK;
}

esp_err_t bsp_lcd_flush(uint16_t x, uint16_t y, uint16_t w, uint16_t h, void *data)
{
    return lcd.draw_bitmap(x, y, w, h, (uint16_t *)data);
}
