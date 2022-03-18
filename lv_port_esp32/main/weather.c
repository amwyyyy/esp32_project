#include <stdio.h>
#include <string.h>
#include "esp_http_client.h"
#include "esp_tls.h"
#include "esp_log.h"
#include "cJSON.h"
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
        .user_agent = "Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1",
        .method = HTTP_METHOD_GET,
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,
        .disable_auto_redirect = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Referer", "http://www.weather.com.cn/");

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "%s", local_response_buffer);

    weather_t wea;
    strtok(local_response_buffer, "=");
    char *dz = strtok(NULL, "=");
    // char *alarm = strtok(NULL, "=");
    // char *sk = strtok(NULL, "=");
    // char *zs = strtok(NULL, "=");
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
    
    return wea;
}