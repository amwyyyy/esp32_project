#ifndef _main_page_h_
#define _main_page_h_

/**
 * @brief 功能列表个数
 *
 */
#define LIST_MAX_FUN 5
/**
 * @brief 开机自动连接wifi
 *
 */
void connect_wifi(void);
/**
 * @brief 壁纸初始化
 *
 * @param num 0 windows 1 天空
 */
void wallpaper_init(uint8_t num);
/**
 * @brief 页面任务
 *
 */
void main_page_task(void);

/**
 * @brief 壁纸定义
 *
 */
#define WALLPAPER_WINDOWS 0 // windows
#define WALLPAPER_SKY 1     // 天空
#define WALLPAPER_LEAF 2    // 树叶
#define WALLPAPER_FLOWER 3  // 花

#endif
