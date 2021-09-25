#include <WiFi.h>
#include "time.h"
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define SUN  0
#define SUN_CLOUD  1
#define CLOUD 2
#define RAIN 3
#define THUNDER 4

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);     

const char* ssid       = "xiongda";
const char* password   = "15999554794";

const char* ntpServer     = "pool.ntp.org";
const long gmtOffset_sec = 3600 * 8;

struct {
  char *localDate;
  char *localTime;
} now;

int minute;
int initFlag = 0;
int updateFlag = 0;

const char* host   = "www.weather.com.cn";
const int httpPort = 80;
String day         = "（今天）";
String weaStart    = "<p title=\"";
String weaEnd      = "\" class";
String highStart   = "<span>";
String highEnd     = "</span>";
String lowStart    = "<i>";
String lowEnd      = "℃</i>";

struct {
  String wea;
  String high;
  String low;
  uint8_t symbol;
  char *weaStr;
} weather;

TaskHandle_t xHandle_update_data = NULL;

// 定义任务
/**
 * @brief 闪灯任务
 * 
 * @param pvParameters 
 */
void TaskBlink(void *pvParameters);

/**
 * @brief 联网更新数据(天气数据、时间同步)
 * 
 * @param pvParameters 
 */
void TaskUpdateData(void *pvParameters);

/**
 * @brief 显示时间
 * 
 * @param pvParameters 
 */
void TaskDrawLocalTime(void *pvParameters);

/**
 * @brief 唤醒联网更新任务
 * 
 * @param pvParameters 
 */
void TaskUpdateWeather(void *pvParameters);

void TaskBlink(void *pvParameters) {
  (void) pvParameters;

  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) {
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(1000);
  }
}

void drawWeatherSymbol(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol) {
  // fonts used:
  // u8g2_font_open_iconic_embedded_6x_t
  // u8g2_font_open_iconic_weather_6x_t
  // encoding values, see: https://github.com/olikraus/u8g2/wiki/fntgrpiconic
  
  switch(symbol)
  {
    case SUN:
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(x, y, 69);  
      break;
    case SUN_CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(x, y, 65); 
      break;
    case CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(x, y, 64); 
      break;
    case RAIN:
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(x, y, 67); 
      break;
    case THUNDER:
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(x, y, 67);
      break;      
  }
}

void getSymbol(String wea) {
  String rain = "雨";
  String cloud = "云";
  String thunder = "雷";
  
  if (wea.indexOf(rain) != -1) {
    weather.symbol = RAIN;
    return;
  }

  if (wea.indexOf(thunder) != -1) {
    weather.symbol = THUNDER;
    return;
  }

  if (wea.indexOf(cloud) != -1) {
    weather.symbol = CLOUD;
    return;
  }

  weather.symbol = SUN;
}

void readWea(WiFiClient client) {
  client.readStringUntil('\n');
  client.readStringUntil('\n');
  String line = client.readStringUntil('\n');
  Serial.println(line);
  weather.wea = line.substring(line.indexOf(weaStart) + 10, line.indexOf(weaEnd)); 
  client.readStringUntil('\n');
  line = client.readStringUntil('\n');
  Serial.println(line);
  weather.high = line.substring(line.indexOf(highStart) + 6, line.indexOf(highEnd));
  weather.low = line.substring(line.indexOf(lowStart) + 3, line.indexOf(lowEnd));

  weather.weaStr = (char *) malloc(strlen(weather.high.c_str()) + strlen(weather.low.c_str()) + 3);
  sprintf(weather.weaStr, "%s-%s°C", weather.low, weather.high);
}

void getWeather() {
  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
  }

  String url = "/weather/101280601.shtml";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
      }
  }

  while(client.available()) {
    String line = client.readStringUntil('\n');
    if (line.indexOf(day) != -1) {
      Serial.println(line);
      readWea(client);   
      break;
    }
  }

  getSymbol(weather.wea);

  Serial.println();
  Serial.println("closing connection");
}

int initLocalTime(){ 
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 0;
  }

  char *ld = (char *) malloc(10);
  sprintf(ld, "%d.%02d.%02d", (timeinfo.tm_year + 1900), (timeinfo.tm_mon + 1), timeinfo.tm_mday);
  now.localDate = ld;

  char *lt = (char *) malloc(8);
  sprintf(lt, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  now.localTime = lt;

  minute = timeinfo.tm_min;

  return 1;
}

void connectWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      vTaskDelay(300);
  }

  Serial.println("");
  Serial.print("WiFi connected. ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void closeWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println();
  Serial.println("closing wifi");
}

void drawLocalTime() {
  initLocalTime();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_7x14_tf);
    u8g2.drawUTF8(25, 12, weather.weaStr);
    drawWeatherSymbol(1, 15, weather.symbol);

    u8g2.setFont(u8g2_font_lubB18_tr);
    u8g2.drawStr(12, 43, now.localTime);

    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr(68, 60, now.localDate);
  } while (u8g2.nextPage());
}

void TaskUpdateData(void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    connectWiFi();

    int t = 0;
    do {
      configTime(gmtOffset_sec, 0, ntpServer);
      vTaskDelay(200);
      t = initLocalTime();
    } while (t == 0);
    Serial.println("config time finish!");

    getWeather();
    Serial.println("getWeather finish!");

    closeWiFi();

    initFlag = 1;

    vTaskSuspend(xHandle_update_data);
  }
}

void TaskDrawLocalTime(void *pvParameters) {
  (void) pvParameters;

  while(initFlag == 0) {
    vTaskDelay(200);
  }

  while (1) {
    initLocalTime();
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_7x14_tf);
      u8g2.drawUTF8(25, 12, weather.weaStr);
      drawWeatherSymbol(1, 15, weather.symbol);

      u8g2.setFont(u8g2_font_lubB18_tr);
      u8g2.drawStr(12, 43, now.localTime);

      u8g2.setFont(u8g2_font_profont11_tr);
      u8g2.drawStr(68, 60, now.localDate);
    } while (u8g2.nextPage());
    vTaskDelay(200);
  }
}

void TaskUpdateWeather(void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    if (minute == 0 && updateFlag == 0) {
      Serial.println("resume update data");
      vTaskResume(xHandle_update_data);
    }

    if (minute != 0) {
      updateFlag = 1;
    }

    vTaskDelay(500);
  }
}

void setup() {
  Serial.begin(115200);

  u8g2.begin();
  u8g2.enableUTF8Print();

  xTaskCreatePinnedToCore(TaskBlink, "TaskBlink", 1024, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  
  xTaskCreatePinnedToCore(TaskUpdateData, "TaskUpdateData", 1024 * 2, NULL, 5, &xHandle_update_data, ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(TaskDrawLocalTime, "TaskDrawLocalTime", 1024 * 2, NULL, 2, NULL, ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(TaskUpdateWeather, "TaskUpdateWeather", 1024, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}

void loop() {

}
