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
#include "task_mag.h"

#define TAG "GPIO"
#define ESP_INTR_FLAG_DEFAULT 0

xQueueHandle queue_button;
static uint32_t button1_backup_count = 0;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    uint32_t i;

    switch (gpio_num)
    {
    case ROLLER_L:
        if (gpio_get_level(ROLLER_R) == 0)
        {
            gpio_num = ROLLER_L;
            xQueueSend(queue_button, &gpio_num, NULL);
        }
        break;
    case BUTTON_1:
        for (i = 0; i < 500; i++)
        {
            if (gpio_get_level(BUTTON_1) != 0)
                break;
        }

        if (i == 500)
        {
            gpio_num = BUTTON_1;
            xQueueSend(queue_button, &gpio_num, NULL);
        }
        break;
    case KEYBOARD_1:
        for (i = 0; i < 500; i++)
        {
            if (gpio_get_level(KEYBOARD_1) != 0)
                break;
        }

        if (i == 500)
        {
            keyboard_sned_key(KEYBOARD_1);
        }
        break;
    case KEYBOARD_2:
        for (i = 0; i < 500; i++)
        {
            if (gpio_get_level(KEYBOARD_2) != 0)
                break;
        }

        if (i == 500)
        {
            gpio_num = KEYBOARD_2;
            xQueueSend(queue_button, &gpio_num, NULL);
        }
        break;
    case KEYBOARD_3:
        for (i = 0; i < 500; i++)
        {
            if (gpio_get_level(KEYBOARD_3) != 0)
                break;
        }

        if (i == 500)
        {
            keyboard_sned_key(KEYBOARD_3);
        }
        break;
    case ROLLER_R:
        if (gpio_get_level(ROLLER_L) == 0)
        {
            gpio_num = ROLLER_R;
            xQueueSend(queue_button, &gpio_num, NULL);
        }
        break;
    default:
        break;
    }
}

uint8_t app_encoder_state = 0;

static void button_task(void *arg)
{
    uint32_t button;
    unsigned int mp3_index = 0;

    for (;;)
    {
        if (xQueueReceive(queue_button, &button, portMAX_DELAY))
        {
            if (xTaskGetTickCount() > button1_backup_count + 10)
            {
                button1_backup_count = xTaskGetTickCount();

                switch (button)
                {
                case ROLLER_L:
                    ESP_LOGE(TAG, "滚轮正转");
                    app_encoder_state = 2;
                    break;
                case BUTTON_1:
                case KEYBOARD_2:
                    ESP_LOGE(TAG, "滚轮/键盘点击");
                    app_encoder_state = 1;
                    break;
                case ROLLER_R:
                    ESP_LOGE(TAG, "滚轮反转");
                    app_encoder_state = 3;
                    break;
                }
            }
        }
    }
}
    
void encoder_init()
{
    gpio_config_t io_conf;

    vTaskDelay(200 / portTICK_RATE_MS);
    // disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    // set as output mode
    io_conf.mode = GPIO_MODE_INPUT;
    // bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = (1 << ROLLER_L) | (1 << ROLLER_R);
    // disable pull-down mode
    io_conf.pull_down_en = 0;
    // disable pull-up mode
    io_conf.pull_up_en = 1;
    // configure GPIO with the given settings
    gpio_config(&io_conf);

    // install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    queue_button = xQueueCreate(1, sizeof(uint32_t));

    // hook isr handler for specific gpio pin
    gpio_isr_handler_add(ROLLER_L, gpio_isr_handler, (void *)ROLLER_L);
    gpio_isr_handler_add(ROLLER_R, gpio_isr_handler, (void *)ROLLER_R);

    // disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_NEGEDGE;
    // set as output mode
    io_conf.mode = GPIO_MODE_INPUT;
    // bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = (1 << BUTTON_1) | (1 << KEYBOARD_1) | (1 << KEYBOARD_2) | (1 << KEYBOARD_3);
    // disable pull-down mode
    io_conf.pull_down_en = 0;
    // disable pull-up mode
    io_conf.pull_up_en = 1;
    // configure GPIO with the given settings
    gpio_config(&io_conf);

    gpio_isr_handler_add(BUTTON_1, gpio_isr_handler, (void *)BUTTON_1);
    gpio_isr_handler_add(KEYBOARD_1, gpio_isr_handler, (void *)KEYBOARD_1);
    gpio_isr_handler_add(KEYBOARD_2, gpio_isr_handler, (void *)KEYBOARD_2);
    gpio_isr_handler_add(KEYBOARD_3, gpio_isr_handler, (void *)KEYBOARD_3);

    xTaskCreate(button_task, "button_task", 1024 * 2, NULL, 10, NULL);
}
