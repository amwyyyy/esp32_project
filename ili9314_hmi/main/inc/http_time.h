#ifndef _http_time_h_
#define _http_time_h_

void http_get_time(void *pvParameters);

struct tm *z05d_mb_storage_get_timeinfo(void);
void z05d_mb_storage_set_time(struct tm *timeinfo);

#endif
