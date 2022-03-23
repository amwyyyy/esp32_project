#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/portmacro.h"
#include "lvgl_helpers.h"
#include "gui.h"

#include "sntp_time.h"
#include "assets/taikongren01.c"
#include "assets/taikongren02.c"
#include "assets/taikongren03.c"
#include "assets/taikongren04.c"
#include "assets/taikongren05.c"
#include "assets/taikongren06.c"
#include "assets/taikongren07.c"
#include "assets/taikongren08.c"
#include "assets/taikongren09.c"
#include "assets/taikongren10.c"
#include "assets/taikongren11.c"
#include "assets/taikongren12.c"
#include "assets/taikongren13.c"
#include "assets/taikongren14.c"
#include "assets/taikongren15.c"

/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#define TAG "gui"
#define LV_TICK_PERIOD_MS 1

#include "lv_font/lv_font.h"
// #include "font/myfont_3500hz_18.c"
// LV_FONT_DECLARE(myfont_3500hz_18);

#include "font/yellow_candy_80.c"
LV_FONT_DECLARE(yellow_candy_80);

#include "font/yellow_candy_40.c"
LV_FONT_DECLARE(yellow_candy_40);

#include "font/dig_60.c"
LV_FONT_DECLARE(dig_60);

#include "font/dig_30.c"
LV_FONT_DECLARE(dig_30);

LV_IMG_DECLARE(taikongren01)
LV_IMG_DECLARE(taikongren02)
LV_IMG_DECLARE(taikongren03)
LV_IMG_DECLARE(taikongren04)
LV_IMG_DECLARE(taikongren05)
LV_IMG_DECLARE(taikongren06)
LV_IMG_DECLARE(taikongren07)
LV_IMG_DECLARE(taikongren08)
LV_IMG_DECLARE(taikongren09)
LV_IMG_DECLARE(taikongren10)
LV_IMG_DECLARE(taikongren11)
LV_IMG_DECLARE(taikongren12)
LV_IMG_DECLARE(taikongren13)
LV_IMG_DECLARE(taikongren14)
LV_IMG_DECLARE(taikongren15)

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);

static lv_obj_t * scene_bg;
static lv_obj_t * hour_label;
static lv_obj_t * minute_label;
static lv_obj_t * second_label;
static uint32_t in = 0;
static lv_obj_t * img1;
static lv_obj_t * loading_label;

void gui_init(void) {
    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
    xTaskCreatePinnedToCore(guiTask, "gui", 2048 * 4, NULL, 0, NULL, 1);
}

void clock_task(lv_task_t * task) {
    date_time_t dt = get_now_time();
    lv_label_set_text_fmt(hour_label, "%02d", dt.hour);
    lv_label_set_text_fmt(minute_label, "%02d", dt.minute);
    lv_label_set_text_fmt(second_label, "%02d", dt.second);
}

void img_task(lv_task_t * task) {
    switch (in++)
    {
    case 0:lv_img_set_src(img1, &taikongren01);break;
    case 1:lv_img_set_src(img1, &taikongren02);break;
    case 2:lv_img_set_src(img1, &taikongren03);break;
    case 3:lv_img_set_src(img1, &taikongren04);break;
    case 4:lv_img_set_src(img1, &taikongren05);break;
    case 5:lv_img_set_src(img1, &taikongren06);break;
    case 6:lv_img_set_src(img1, &taikongren07);break;
    case 7:lv_img_set_src(img1, &taikongren08);break;
    case 8:lv_img_set_src(img1, &taikongren09);break;
    case 9:lv_img_set_src(img1, &taikongren10);break;
    case 10:lv_img_set_src(img1, &taikongren11);break;
    case 11:lv_img_set_src(img1, &taikongren12);break;
    case 12:lv_img_set_src(img1, &taikongren13);break;
    case 13:lv_img_set_src(img1, &taikongren14);break;
    case 14:lv_img_set_src(img1, &taikongren15);break;
    default:
        break;
    }

    if (in >= 14) {
        in = 0;
    }
}

void set_loading_text(const char * text) {
    lv_label_set_text(loading_label, text);
}

void display(display_type_t type) {
    static lv_style_t bg_style;
    lv_style_init(&bg_style);	
    lv_style_set_bg_color(&bg_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_obj_t * scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_add_style(scr, LV_OBJ_PART_MAIN, &bg_style);

    if (type == DISP_CLOCK) {
        lv_obj_t * scene_main = lv_obj_create(scr, NULL);
        lv_obj_reset_style_list(scene_main, LV_OBJ_PART_MAIN);
        lv_obj_set_size(scene_main, lv_obj_get_width(scr), lv_obj_get_height(scr));
        lv_obj_align(scene_main, NULL, LV_ALIGN_CENTER, 0, 0);

        // 时间显示
        scene_bg = lv_obj_create(scene_main, NULL);
        lv_obj_reset_style_list(scene_bg, LV_OBJ_PART_MAIN);
        lv_obj_set_size(scene_bg, lv_obj_get_width(scr), lv_obj_get_height(scr) / 2);
        lv_obj_align(scene_bg, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

        static lv_style_t hour_style;
        lv_style_init(&hour_style);	
        lv_style_set_text_font(&hour_style, LV_STATE_DEFAULT, &dig_60);
        lv_style_set_text_color(&hour_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

        hour_label = lv_label_create(scene_bg, NULL);
        lv_label_set_long_mode(hour_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(hour_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 5, -50);
        lv_obj_add_style(hour_label, LV_LABEL_PART_MAIN, &hour_style);

        static lv_style_t minute_style;
        lv_style_init(&minute_style);	
        lv_style_set_text_font(&minute_style, LV_STATE_DEFAULT, &dig_60);
        lv_style_set_text_color(&minute_style, LV_STATE_DEFAULT, LV_COLOR_ORANGE);

        minute_label = lv_label_create(scene_bg, NULL);
        lv_label_set_long_mode(minute_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(minute_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 87, -50);
        lv_obj_add_style(minute_label, LV_LABEL_PART_MAIN, &minute_style);

        static lv_style_t second_style;
        lv_style_init(&second_style);	
        lv_style_set_text_font(&second_style, LV_STATE_DEFAULT, &dig_30);
        lv_style_set_text_color(&second_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

        second_label = lv_label_create(scene_bg, NULL);
        lv_label_set_long_mode(second_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(second_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 187, -29);
        lv_obj_add_style(second_label, LV_LABEL_PART_MAIN, &second_style);

        lv_task_t * task = lv_task_create(clock_task, 1000, LV_TASK_PRIO_MID, NULL);
        lv_task_ready(task);

        // 图片显示
        lv_obj_t * scene_img = lv_obj_create(scene_main, NULL);
        lv_obj_reset_style_list(scene_img, LV_OBJ_PART_MAIN);
        lv_obj_set_size(scene_img, lv_obj_get_width(scr), lv_obj_get_height(scr) / 2);
        lv_obj_align(scene_img, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

        img1 = lv_img_create(scene_img, NULL);
        lv_obj_align(img1, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, -34);

        lv_task_t * imgDisp = lv_task_create(img_task, 80, LV_TASK_PRIO_MID, NULL);
        lv_task_ready(imgDisp);
    } else if (type == DISP_LOADING) {
        lv_obj_t * preload = lv_spinner_create(scr, NULL);
        lv_obj_set_size(preload, 100, 150);
        lv_obj_align(preload, NULL, LV_ALIGN_CENTER, 0, 0);

        static lv_style_t label_style;
        lv_style_init(&label_style);	
        lv_style_set_text_color(&label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

        loading_label = lv_label_create(preload, NULL);
        lv_label_set_long_mode(loading_label, LV_LABEL_LONG_EXPAND);
        lv_obj_align(loading_label, NULL, LV_ALIGN_IN_BOTTOM_MID, -20, 0);
        lv_obj_add_style(loading_label, LV_LABEL_PART_MAIN, &label_style);
        
        set_loading_text("Loading...");
    }
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

    display(DISP_LOADING);

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

static void lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}
