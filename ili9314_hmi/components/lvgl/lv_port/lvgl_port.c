/**
 * @file lvgl_port.c
 * @brief LVGL required fuctions
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

#include "lvgl_port.h"

static const char *TAG = "lvgl_port";
static SemaphoreHandle_t lvgl_mutex = NULL;
static bool lv_port_default_handler_en = true;

esp_err_t lv_port_sem_take(void)
{
    return !xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
}

esp_err_t lv_port_sem_give(void)
{
    return !xSemaphoreGive(lvgl_mutex);
}

static void lvgl_tick_task(void *data)
{
    static TickType_t tick = 0;
    static const uint32_t task_period = 5;

    tick = xTaskGetTickCount();

    while (1)
    {
        lv_tick_inc(task_period);
        vTaskDelayUntil(&tick, pdMS_TO_TICKS(task_period));
    }
}

void lv_handler_task(void *pvparam)
{
    static TickType_t tick;

    tick = xTaskGetTickCount();

    while (1)
    {
        lv_port_sem_take();
        lv_task_handler();
        lv_port_sem_give();

        vTaskDelayUntil(&tick, pdMS_TO_TICKS(20));
    }
}

static IRAM_ATTR void lvgl_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lv_disp_flush_ready(disp_drv);

    bsp_lcd_flush(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)color_p);
}

static IRAM_ATTR bool lvgl_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint8_t tp_num = 0;
    static uint16_t x = 0, y = 0;

    ft5x06_read_pos(&tp_num, &x, &y);

    if (0 == tp_num)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        /* Swap xy order and invert y direction */
        data->point.x = y;
        data->point.y = LV_VER_RES_MAX - x - 1;
        data->state = LV_INDEV_STATE_PR;
    }

    return false;
}

#if 1

static int32_t encoder_diff;
static lv_indev_state_t encoder_state;

static lv_indev_t *indev_encoder = NULL;
static lv_group_t *g = NULL;
static bool encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

lv_indev_t *Get_indev(void);
lv_group_t *Get_group(void);

void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv);
    g = lv_group_create();
    lv_indev_set_group(indev_encoder, g);

    if (indev_encoder == NULL)
    {
        ESP_LOGE(TAG, "Failed to register input device.");
    }
}

static bool encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint32_t last_key = 0;
    extern uint8_t app_encoder_state;

    uint32_t act_key = app_encoder_state;
    if (act_key != 0)
    {
        switch (act_key)
        {
        case 1:
            act_key = LV_KEY_ENTER;
            encoder_state = LV_INDEV_STATE_PR;
            break;
        case 2:
            act_key = LV_KEY_LEFT;
            encoder_diff = -1;
            encoder_state = LV_INDEV_STATE_REL;
            break;
        case 3:
            act_key = LV_KEY_RIGHT;
            encoder_state = LV_INDEV_STATE_REL;
            encoder_diff = 1;
            break;
        }
        last_key = act_key;
        app_encoder_state = 0;
    }
    else
    {
        encoder_diff = 0;
        encoder_state = LV_INDEV_STATE_REL;
    }
    data->key = last_key;
    data->enc_diff = encoder_diff;
    data->state = encoder_state;
    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

lv_indev_t *Get_indev(void)
{
    return indev_encoder;
}
lv_group_t *Get_group(void)
{
    return g;
}

#endif

esp_err_t lvgl_init(size_t buffer_pix_size, uint32_t buffer_caps)
{
    lv_init();

    lvgl_mutex = xSemaphoreCreateMutex();

    /* Create a display buffer */
    static lv_disp_buf_t disp_buf;
    static DRAM_ATTR lv_color_t *lv_buf;

    /*!< Use DRAM as sigle none screen size buffer */
    lv_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * buffer_pix_size, buffer_caps);
    lv_disp_buf_init(&disp_buf, lv_buf, NULL, buffer_pix_size);

    if (NULL == lv_buf)
    {
        ESP_LOGE(TAG, "No free mem for allocating buffer");
        return ESP_ERR_NO_MEM;
    }

    /*Create a display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = lvgl_flush_cb;
    lv_disp_drv_register(&disp_drv);

    // /*!< Register input devcie and callback */
    // lv_indev_drv_t indev_drv;
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // indev_drv.read_cb = lvgl_read_cb;
    // lv_indev_drv_register(&indev_drv);

    lv_port_indev_init();

    if (pdPASS != xTaskCreate(
                      (TaskFunction_t)lvgl_tick_task,
                      (const char *const)"LVGL Tick Task",
                      (const uint32_t)512,
                      (void *const)NULL,
                      (UBaseType_t)configMAX_PRIORITIES - 1,
                      (TaskHandle_t *const)NULL))
    {
        return ESP_ERR_NO_MEM;
    }

    /* Task for lvgl event handler and screen flush */
    if (lv_port_default_handler_en)
    {
        if (pdPASS != xTaskCreate(
                          (TaskFunction_t)lv_handler_task,
                          (const char *const)"LVGL Handler Task",
                          (const uint32_t)4 * 1024,
                          (void *const)NULL,
                          (UBaseType_t)configMAX_PRIORITIES - 3,
                          (TaskHandle_t *const)NULL))
        {
            return ESP_ERR_NO_MEM;
        }
    }

    return ESP_OK;
}

void lv_port_use_default_handler(bool en)
{
    lv_port_default_handler_en = en;
}
