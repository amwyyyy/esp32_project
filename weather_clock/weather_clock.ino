#include <WiFi.h>
#include <time.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <DHTesp.h>
#include <Preferences.h>
#include <images.h>
#include <WebServer.h>
#include <uri/UriBraces.h>

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

#define SUN       0
#define SUN_CLOUD 1
#define CLOUD     2
#define RAIN      3
#define THUNDER   4

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);     

const char* ntpServer    = "ntp.aliyun.com";
const long gmtOffset_sec = 3600 * 8;

struct {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} now;

int initFlag = 0;
int updateFlag = 0;
int viewFlag = 0;

// 天气相关变量
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
} weather;

TaskHandle_t xHandle_update_data = NULL;

// 温湿度相关
DHTesp dht;
int dhtPin = 4;
TempAndHumidity newValues = {.temperature=0.0, .humidity=0.0};

Preferences preferences;
String ssid;
String password;
WebServer server(80);

// 定义任务
// 闪灯任务
void TaskBlink(void *pvParameters);
// 联网更新数据(天气数据、时间同步)
void TaskUpdateData(void *pvParameters);
// 显示时间
void TaskDrawLocalTime(void *pvParameters);
// 唤醒联网更新任务
void TaskUpdateWeather(void *pvParameters);
// 获取温湿度任务
void TaskGetTemperature(void *pvParameters);
// 切换温度天气显示
void TaskChangeView(void *pvParameters);
// 显示配网二维码
void TaskDisplayQRCode(void *pvParameters);

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
      u8g2.setFont(u8g2_font_open_iconic_weather_1x_t);
      u8g2.drawGlyph(x, y, 69);  
      break;
    case SUN_CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_1x_t);
      u8g2.drawGlyph(x, y, 65); 
      break;
    case CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_1x_t);
      u8g2.drawGlyph(x, y, 64); 
      break;
    case RAIN:
      u8g2.setFont(u8g2_font_open_iconic_weather_1x_t);
      u8g2.drawGlyph(x, y, 67); 
      break;
    case THUNDER:
      u8g2.setFont(u8g2_font_open_iconic_weather_1x_t);
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

void readWeather(WiFiClient client) {
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
      readWeather(client);   
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

  now.year = timeinfo.tm_year + 1900;
  now.month = timeinfo.tm_mon + 1;
  now.day = timeinfo.tm_mday;
  now.hour = timeinfo.tm_hour;
  now.minute = timeinfo.tm_min;
  now.second = timeinfo.tm_sec;

  return 1;
}

void connectWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid.c_str(), password.c_str());

  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
      vTaskDelay(300);
      if (count++ > 200) {
        preferences.clear();
        ESP.restart();
      }
  }

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
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_7x14_tr);
      u8g2.drawStr(20, 20, "System init.");
    } while (u8g2.nextPage());
    vTaskDelay(200);
  }

  // 接收格式化字符串
  char *ld     = (char *) malloc(11);
  char *lt     = (char *) malloc(10);
  char *temper = (char *) malloc(6);
  char *humid  = (char *) malloc(5);
  char *weaStr = (char *) malloc(11);

  while (1) {
    initLocalTime();
    u8g2.firstPage();
    do {
      if (viewFlag) {
        if (atoi(weather.high.c_str()) == 0) {
          sprintf(weaStr, "%s°C", weather.low);
        } else {
          sprintf(weaStr, "%s/%s°C", weather.low, weather.high);
        }
        u8g2.setFont(u8g2_font_profont11_tf);
        u8g2.drawUTF8(15, 12, weaStr);
        drawWeatherSymbol(1, 12, weather.symbol);
      } else {
        u8g2.setFont(u8g2_font_profont11_tf);
        sprintf(temper, "%0.1f°C", newValues.temperature);
        u8g2.drawUTF8(3, 12, temper);
      
        sprintf(humid, " / %0.0f%%", newValues.humidity);
        u8g2.drawUTF8(40, 12, humid);
      }

      u8g2.setFont(u8g2_font_lubB18_tr);
      sprintf(lt, "%02d:%02d:%02d", now.hour, now.minute, now.second);
      u8g2.drawStr(12, 43, lt);

      u8g2.setFont(u8g2_font_profont11_tr);
      sprintf(ld, "%d.%02d.%02d", now.year, now.month, now.day);
      u8g2.drawStr(68, 60, ld);
    } while (u8g2.nextPage());
    vTaskDelay(200);
  }
}

void TaskUpdateWeather(void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    if (now.minute == 0 && updateFlag == 1) {
      Serial.println("resume update data");
      vTaskResume(xHandle_update_data);
      updateFlag = 0;
    }

    if (now.minute != 0) {
      updateFlag = 1;
    }

    vTaskDelay(500);
  }
}

void TaskGetTemperature(void *pvParameters) {
  for(;;) {
    newValues = dht.getTempAndHumidity();
    if (dht.getStatus() != 0) {
      Serial.println("DHT11 error status: " + String(dht.getStatusString()));
      vTaskDelay(500);
      continue;
    }
    vTaskDelay(1000 * 10);
  }
}

void TaskChangeView(void *pvParameters) {
  for (;;) {
    viewFlag = !viewFlag;
    vTaskDelay(8000);  
  }
}

void TaskDisplayQRCode(void *pvParameters) {
  for (;;) {
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawXBMP(32, 0, qrcode_width, qrcode_height, qrcode_bits);
    } while (u8g2.nextPage());
    vTaskDelay(500);
  }
}

// 配置 WiFi 账号密码
void configWiFi() {
  xTaskCreatePinnedToCore(TaskDisplayQRCode, "TaskDisplayQRCode", 1024 * 4, NULL, 3, NULL, ARDUINO_RUNNING_CORE);

  const char *ssidd = "esp32-clock";
  WiFi.softAP(ssidd);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on(UriBraces("/setting/{}/{}"), []() {
    String ssidA = server.pathArg(0);
    String passwordA = server.pathArg(1);
    if (ssidA.length() !=0 && passwordA.length() != 0) {
      preferences.putString("ssid", ssidA);
      preferences.putString("password", passwordA);
      server.send(200, "text/html", returnHtml);
      delay(1000);
      ESP.restart();
    } else {
      server.send(400, "text/plain", "input error!");
    }
  });

  server.on("/", []() {
    server.send(200, "text/html", html);
  });
  
  server.begin();

  Serial.println("Server started");
}

void setup() {
  Serial.begin(115200);

  u8g2.begin();
  u8g2.enableUTF8Print();

  dht.setup(dhtPin, DHTesp::DHT11);

  preferences.begin("esp32-clock", false);
  ssid = preferences.getString("ssid");
  password = preferences.getString("password");
  preferences.end();

  // 根据是否获取到 WiFi 账号来判断是否进入配网模式
  if (ssid.length() == 0) {
    configWiFi();
  } else {
    // xTaskCreatePinnedToCore(TaskBlink, "TaskBlink", 1024 * 2, NULL, 5, NULL, ARDUINO_RUNNING_CORE);
    
    xTaskCreatePinnedToCore(TaskUpdateData, "TaskUpdateData", 1024 * 4, NULL, 1, &xHandle_update_data, ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(TaskDrawLocalTime, "TaskDrawLocalTime", 1024 * 4, NULL, 1, NULL, ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(TaskUpdateWeather, "TaskUpdateWeather", 1024 * 2, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
    
    xTaskCreatePinnedToCore(TaskGetTemperature, "TaskGetTemperature", 1024, NULL, 3, NULL, ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(TaskChangeView, "TaskChangeView", 1024, NULL, 4, NULL, ARDUINO_RUNNING_CORE);
  }
}

void loop() {
  if (ssid.length() == 0) {
    server.handleClient();
    delay(50);
  } else {
    // 非配网状态防止空转
    delay(5000);
  }
}
