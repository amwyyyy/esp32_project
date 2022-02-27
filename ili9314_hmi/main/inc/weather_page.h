#ifndef _weather_page_h_
#define _weather_page_h_

/**
 * @brief 页面任务
 * 
 */
void weather_page_task(void);

typedef struct
{
    uint32_t numtq;      //天气现象编码
    char cityId[16];     //城市id
    char qw[8];          //当前气温
    char sd[16];         //相对湿度，直接在此数值后添加%即可
    char lastUpdate[32]; //最后一次更新时间
} weather_data_t;

#endif
