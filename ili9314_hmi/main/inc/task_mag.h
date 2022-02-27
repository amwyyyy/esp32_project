#ifndef _task_mag_h_
#define _task_mag_h_

typedef enum
{
    NUMM_TASK_NUM,         //空
    MAIN_PAGE_TASK_NUM,    //主页（该页面不能删除，需要维持运行）
    WIFI_SCAN_TASK_NUM,    // wifi搜索页面
    WEATHER_PAGE_TASK_NUM, //天气页面
    MUSIC_PAGE_TASK_NUM,   //音乐页面
    SETTING_PAGE_TASK_NUM, //设置页面
} PAGE_TASK;

/**
 * @brief 启动页面任务
 *
 * @param num
 */
void cql_page_init(PAGE_TASK num);
/**
 * @brief  发送键盘键值
 *
 * @param key 键值
 */
void keyboard_sned_key(uint32_t key);
/**
 * @brief 删除页面任务
 *
 * @param num
 */
void cql_page_dele(PAGE_TASK num);

#endif
