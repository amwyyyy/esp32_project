#include <stdio.h>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "sntp_time.h"

#define TAG "sntp"

esp_err_t sntp_time_init(void) {
    ESP_LOGI(TAG, "Initializing SNTP");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);  
	sntp_setservername(0, "ntp.aliyun.com"); 
    sntp_setservername(1, "ntp1.aliyun.com"); 
	sntp_init();

    // 设置时区
    setenv("TZ", "CST-8", 1);
	tzset();

    time_t now = 0;
    struct tm timeinfo = {0};

    int s_retry_num = 0;

    do {
        if (s_retry_num >= 10) {
            return ESP_ERR_TIMEOUT;
        }
        time(&now);
        localtime_r(&now, &timeinfo);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    } while (timeinfo.tm_year < 100);

    ESP_LOGI(TAG, "Finish SNTP");

    return ESP_OK;
}

date_time get_now_time(void) {
    time_t now = 0;
    struct tm timeinfo = {0};
    date_time dt;

    time(&now);
    localtime_r(&now, &timeinfo);

    dt.year   = timeinfo.tm_year + 1900;
    dt.month  = timeinfo.tm_mon + 1;
    dt.day    = timeinfo.tm_mday;
    dt.hour   = timeinfo.tm_hour;
    dt.minute = timeinfo.tm_min;
    dt.second = timeinfo.tm_sec;

    ESP_LOGI(TAG, "current time: %d-%d-%d %d:%d:%d",
			 dt.year, dt.month,
			 dt.day, dt.hour,
			 dt.minute, dt.second); 

    return dt;       
}