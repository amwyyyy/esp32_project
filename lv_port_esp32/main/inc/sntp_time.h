#ifndef _sntp_h_
#define _sntp_h_

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} date_time;

/**
 * @brief 初始化 sntp
 *
 */
esp_err_t sntp_time_init(void);

/**
 * @brief 获取当前时间
 */
date_time get_now_time(void);

#endif