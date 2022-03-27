#include <stdio.h>
#include <string.h>
#include "esp_http_client.h"
#include "esp_tls.h"
#include "esp_log.h"
#include "cJSON.h"
#include "lv_misc/lv_color.h"
#include "weather.h"

static const char *TAG = "weather";

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            /*
             *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
             *  However, event handler can also be used in case chunked encoding is used.
             */
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                if (evt->user_data) {
                    memcpy(evt->user_data + output_len, evt->data, evt->data_len);
                } else {
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(esp_http_client_get_content_length(evt->client));
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    memcpy(output_buffer + output_len, evt->data, evt->data_len);
                }
                output_len += evt->data_len;
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                if (output_buffer != NULL) {
                    free(output_buffer);
                    output_buffer = NULL;
                }
                output_len = 0;
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            break;
    }
    return ESP_OK;
}

weather_t weather_init(void) {
    char local_response_buffer[5120] = {0};

    esp_http_client_config_t config = {
        .host = "d1.weather.com.cn",
        .path = "/weather_index/101280601.html",
        // .user_agent = "Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1",
        .method = HTTP_METHOD_GET,
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,
        .disable_auto_redirect = true,
    };

    weather_t wea;

    int s_retry_num = 0;
    while (1) {
        esp_http_client_handle_t client = esp_http_client_init(&config);
        esp_http_client_set_header(client, "Referer", "http://www.weather.com.cn/");

        esp_err_t err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
            break;
        } else {
            ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
            s_retry_num++;
        }

        if (s_retry_num >= 3) {
            wea.high = "";
            wea.low = "";
            wea.weather = "";
            return wea;
        }
    }

    ESP_LOGD(TAG, "%s", local_response_buffer);

    strtok(local_response_buffer, "=");
    char *dz = strtok(NULL, "=");
    char *alarm = strtok(NULL, "=");
    char *sk = strtok(NULL, "=");
    char *zs = strtok(NULL, "=");
    char *fc = strtok(NULL, "=");

    cJSON *dz_json = cJSON_Parse(dz);
    if (dz_json == NULL) {
        ESP_LOGE(TAG, "json parse error: %s", cJSON_GetErrorPtr());
        vTaskDelete(NULL);
    }
    cJSON *weatherinfo = cJSON_GetObjectItemCaseSensitive(dz_json, "weatherinfo");
    cJSON *weather = cJSON_GetObjectItemCaseSensitive(weatherinfo, "weather");
    if (cJSON_IsString(weather) && (weather->valuestring != NULL)) {
        wea.weather = weather->valuestring;
    }
    cJSON *weathercode = cJSON_GetObjectItemCaseSensitive(weatherinfo, "weathercode");
    if (cJSON_IsString(weathercode) && (weathercode->valuestring != NULL)) {
        wea.weathercode = weathercode->valuestring;
    }
    cJSON *weathercoden = cJSON_GetObjectItemCaseSensitive(weatherinfo, "weathercoden");
    if (cJSON_IsString(weathercoden) && (weathercoden->valuestring != NULL)) {
        wea.weathercoden = weathercoden->valuestring;
    }
    cJSON *city = cJSON_GetObjectItemCaseSensitive(weatherinfo, "city");
    if (cJSON_IsString(city) && (city->valuestring != NULL)) {
        wea.city = city->valuestring;
    }

    cJSON *sk_json = cJSON_Parse(sk);
    cJSON *sk_temp = cJSON_GetObjectItemCaseSensitive(sk_json, "temp");
    if (cJSON_IsString(sk_temp) && (sk_temp->valuestring != NULL)) {
        wea.temp = sk_temp->valuestring;
    }
    cJSON *sk_sd = cJSON_GetObjectItemCaseSensitive(sk_json, "SD");
    if (cJSON_IsString(sk_sd) && (sk_sd->valuestring != NULL)) {
        wea.sd = sk_sd->valuestring;
    }
    cJSON *sk_aqi = cJSON_GetObjectItemCaseSensitive(sk_json, "aqi");
    if (cJSON_IsString(sk_aqi) && (sk_aqi->valuestring != NULL)) {
        wea.aqi = strtol(sk_aqi->valuestring, NULL, 10);
    }

    cJSON *fc_json = cJSON_Parse(fc);
    cJSON *f = cJSON_GetObjectItemCaseSensitive(fc_json, "f");
    cJSON *ff = cJSON_DetachItemFromArray(f, 0);

    cJSON *low = cJSON_GetObjectItemCaseSensitive(ff, "fd");
    if (cJSON_IsString(low) && (low->valuestring != NULL)) {
        wea.low = low->valuestring;
    }
    cJSON *high = cJSON_GetObjectItemCaseSensitive(ff, "fc");
    if (cJSON_IsString(high) && (high->valuestring != NULL)) {
        wea.high = high->valuestring;
    }

    ESP_LOGI(TAG, "init weather success.");

    return wea;
}

void get_aqi_level(uint32_t aqi, char * aqi_level, uint32_t * bg_color) {
    if (aqi >= 301) {
        strcpy(aqi_level, "严重");
        *bg_color = 0xFF2400;
    } else if (aqi >= 201 && aqi <= 300) {
        strcpy(aqi_level, "重度");
        *bg_color = 0x880B20;
    } else if (aqi >= 161 && aqi <= 200) {
        strcpy(aqi_level, "中度");
        *bg_color = 0xBA3779;
    } else if (aqi >= 101 && aqi <= 160) {
        strcpy(aqi_level, "轻度");
        *bg_color = 0xF29F39;
    } else if (aqi >= 51 && aqi <= 100) {
        strcpy(aqi_level, "良");
        *bg_color = 0xF7DB64;
    } else if (aqi > 0 && aqi <= 50) {
        strcpy(aqi_level, "优");
        *bg_color = 0x9CCA64;
    }
}