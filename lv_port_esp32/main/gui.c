#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/portmacro.h"
#include "lvgl_helpers.h"

#include "sntp_time.h"
#include "assets/1.c"
#include "assets/2.c"
#include "assets/3.c"
#include "assets/4.c"
#include "assets/5.c"
#include "assets/6.c"
#include "assets/7.c"
#include "assets/8.c"
#include "assets/9.c"

/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#define TAG "gui"
#define LV_TICK_PERIOD_MS 1

// #include "font/myfont_3500hz_18.c"
// LV_FONT_DECLARE(myfont_3500hz_18);
// #include "lv_font_montserrat_30.c"
// LV_FONT_DECLARE(lv_font_montserrat_30);
LV_IMG_DECLARE(tkr1)
LV_IMG_DECLARE(tkr2)
LV_IMG_DECLARE(tkr3)
LV_IMG_DECLARE(tkr4)
LV_IMG_DECLARE(tkr5)
LV_IMG_DECLARE(tkr6)
LV_IMG_DECLARE(tkr7)
LV_IMG_DECLARE(tkr8)
LV_IMG_DECLARE(tkr9)

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);

void gui_init(void) {
    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
    xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 0, NULL, tskNO_AFFINITY);
}

static lv_obj_t * scene_bg;
static lv_obj_t * scene_img;

void clock_task(lv_task_t * task) {
    lv_obj_clean(scene_bg);

    lv_obj_t * label = lv_label_create(scene_bg, NULL);
    lv_label_set_recolor(label, true);
    lv_label_set_long_mode(label, LV_LABEL_LONG_EXPAND);
    lv_obj_align(label, NULL, LV_ALIGN_IN_BOTTOM_MID, -60, 0);

    date_time_t dt = get_now_time();
    lv_label_set_text_fmt(label, "%d-%02d-%02d %02d:%02d:%02d", 
        dt.year, dt.month,
	    dt.day, dt.hour,
		dt.minute, dt.second);  

    // static lv_style_t bg_style;
    // lv_style_init(&bg_style);	
    // lv_style_set_text_font(&bg_style, LV_STATE_DEFAULT, &myfont_3500hz_18);
	// lv_style_set_text_color(&bg_style, LV_STATE_DEFAULT, LV_COLOR_RED);

    // lv_obj_t * label = lv_label_create(obj1, NULL);
    // // lv_label_set_recolor(label, true);
    // lv_label_set_long_mode(label, LV_LABEL_LONG_EXPAND);
    // lv_obj_align(label, NULL, LV_ALIGN_CENTER, -40, 0);
    // lv_obj_add_style(label, LV_LABEL_PART_MAIN, &bg_style);
}

static uint32_t in = 0;
static lv_obj_t * img1;
void img_task(lv_task_t * task) {
    lv_obj_align(img1, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    switch (in++)
    {
    case 0:lv_img_set_src(img1, &tkr1);break;
    case 1:lv_img_set_src(img1, &tkr2);break;
    case 2:lv_img_set_src(img1, &tkr3);break;
    case 3:lv_img_set_src(img1, &tkr4);break;
    case 4:lv_img_set_src(img1, &tkr5);break;
    case 5:lv_img_set_src(img1, &tkr6);break;
    case 6:lv_img_set_src(img1, &tkr7);break;
    case 7:lv_img_set_src(img1, &tkr8);break;
    case 8:lv_img_set_src(img1, &tkr9);break;
    default:
        break;
    }

    if (in >= 8) {
        in = 0;
    }
}

void display() {
    lv_obj_t * scr = lv_scr_act();
    scene_bg = lv_obj_create(scr, NULL);
    lv_obj_reset_style_list(scene_bg, LV_OBJ_PART_MAIN);
    lv_obj_set_size(scene_bg, lv_obj_get_width(scr), lv_obj_get_height(scr)/2);
    lv_obj_align(scene_bg, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    scene_img = lv_obj_create(scr, NULL);
    lv_obj_reset_style_list(scene_img, LV_OBJ_PART_MAIN);
    lv_obj_set_size(scene_img, lv_obj_get_width(scr), lv_obj_get_height(scr)/2);
    lv_obj_align(scene_img, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_task_t * task = lv_task_create(clock_task, 1000, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(task);

    img1 = lv_img_create(scene_img, NULL);
    lv_task_t * imgDisp = lv_task_create(img_task, 100, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(imgDisp);
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

    display();

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
