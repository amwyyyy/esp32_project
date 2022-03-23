#ifndef _event_h_
#define _event_h_

typedef enum {
    EVENT_WIFI_STA_CONNECTED = 0,
    EVENT_WIFI_STA_DISCONNECTED,
    EVENT_WIFI_STA_START,
    EVENT_SNTP_INIT,
    EVENT_SINT_FAILURE

} basic_event_t;

#endif