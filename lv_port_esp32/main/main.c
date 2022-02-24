/* LVGL Example project
 *
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"

/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "lvgl_helpers.h"

#include "iboxchain-logo.c"
#include "myfont_3500hz_18.c"

/*********************
 *      DEFINES
 *********************/
#define TAG "clock"
#define LV_TICK_PERIOD_MS 1

LV_FONT_DECLARE(myfont_3500hz_18);

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);
static void func_test();

/**********************
 *   APPLICATION MAIN
 **********************/
void app_main() {

    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
    xTaskCreatePinnedToCore(guiTask, "gui", 4096, NULL, 0, NULL, 1);
}

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;

static void guiTask(void *pvParameter) {

    (void) pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();

    /* Initialize SPI or I2C bus used by the drivers */
    lvgl_driver_init();

    lv_color_t* buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);

    /* Use double buffered when not working with monochrome displays */
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    lv_color_t* buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2 != NULL);
#else
    static lv_color_t *buf2 = NULL;
#endif

    static lv_disp_buf_t disp_buf;

    uint32_t size_in_px = DISP_BUF_SIZE;

    /* Initialize the working buffer depending on the selected display.
     * NOTE: buf2 == NULL when using monochrome displays. */
    lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;

    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    /* Create the demo application */
    // create_demo_application();
    func_test();

    while (1) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
       }
    }

    /* A task should NEVER return */
    free(buf1);
#ifndef CONFIG_LV_TFT_DISPLAY_MONOCHROME
    free(buf2);
#endif
    vTaskDelete(NULL);
}

LV_IMG_DECLARE(iboxchain_logo)

static void func_test() {
    // lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);
    // lv_label_set_text(label1, "Hello world");
    // lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

    // lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);
    // lv_obj_set_pos(btn, 10, 10);
    // lv_obj_set_size(btn, 120, 50);
    // lv_obj_t * label = lv_label_create(btn, NULL);
    // lv_label_set_text(label, "Button 1");

    // lv_obj_t * slider = lv_slider_create(lv_scr_act(), NULL);
    // lv_obj_set_width(slider, 150); 
    // lv_obj_align(slider, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -40);

    // label = lv_label_create(lv_scr_act(), NULL);
    // lv_label_set_text(label, "0");
    // lv_obj_set_auto_realign(slider, true);
    // lv_obj_align(label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    // static lv_style_t style_line;
    // lv_style_init(&style_line);
    // lv_style_set_line_width(&style_line, LV_STATE_DEFAULT, 2);

    // static lv_point_t line_points[] = { {0, 70}, {240, 70} };
    // lv_obj_t * line1;
    // line1 = lv_line_create(lv_scr_act(), NULL);
    // lv_line_set_points(line1, line_points, 2);
    // lv_obj_add_style(line1, LV_LINE_PART_MAIN, &style_line);

    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &iboxchain_logo);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -20);

    static lv_style_t bg_style;
    lv_style_init(&bg_style);	
    lv_style_set_text_font(&bg_style, LV_STATE_DEFAULT, &myfont_3500hz_18); 
	lv_style_set_text_color(&bg_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label1, "诚信 利他 协作 创新");
    lv_obj_add_style(label1, LV_LABEL_PART_MAIN, &bg_style);
    lv_obj_align(label1, img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
}

static void lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}
