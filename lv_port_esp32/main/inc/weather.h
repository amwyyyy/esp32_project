#ifndef _weather_h_
#define _weather_h_

typedef struct {
    char* weather;
    char* high;
    char* low;
} weather_t;

/**
 * @brief 初始化天气信息
 *
 */
weather_t weather_init(void);

#endif