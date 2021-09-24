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

#define SUN  0
#define SUN_CLOUD  1
#define CLOUD 2
#define RAIN 3
#define THUNDER 4

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);     

const char* ssid       = "xiongda";
const char* password   = "15999554794";

const char* ntpServer     = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 8;
const char* localDate;
const char* localTime;

int minute;
int flag = 0;

const char* host   = "www.weather.com.cn";
const int httpPort = 80;
String day         = "（今天）";
String weaStart    = "<p title=\"";
String weaEnd      = "\" class";
String highStart   = "<span>";
String highEnd     = "</span>";
String lowStart    = "<i>";
String lowEnd      = "℃</i>";
String wea,high,low;
uint8_t symbol;

void drawWeatherSymbol(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol) {
  // fonts used:
  // u8g2_font_open_iconic_embedded_6x_t
  // u8g2_font_open_iconic_weather_6x_t
  // encoding values, see: https://github.com/olikraus/u8g2/wiki/fntgrpiconic
  
  switch(symbol)
  {
    case SUN:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 69);  
      break;
    case SUN_CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 65); 
      break;
    case CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 64); 
      break;
    case RAIN:
      u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
      u8g2.drawGlyph(x, y, 67); 
      break;
    case THUNDER:
      u8g2.setFont(u8g2_font_open_iconic_embedded_6x_t);
      u8g2.drawGlyph(x, y, 67);
      break;      
  }
}

void drawWeather(uint8_t symbol, int degree, int degreeHigh, int x) {
  drawWeatherSymbol(x, 48, symbol);

  int offset = 0;
  if (degreeHigh == 0) {
    u8g2.setFont(u8g2_font_logisoso32_tf);
    offset = 10;
  } else {
    u8g2.setFont(u8g2_font_logisoso16_tf);
  }
  u8g2.setCursor(48 + 3 + x, 32 + offset);
  u8g2.print(degree);
  if (degreeHigh != 0) {
    u8g2.print("~");
    u8g2.print(degreeHigh);
  }
  u8g2.print("°C");
}

void draw(const char *s, int degree, int degreeHigh, int x){
  int16_t len = strlen(s);
  u8g2.firstPage();
  do {
    drawWeather(symbol, degree, degreeHigh, x);
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2.drawUTF8((60 - 2*len) + x, 62, s);
  } while ( u8g2.nextPage() );
}

void getSymbol(String wea) {
  String rain = "雨";
  String cloud = "云";
  String thunder = "雷";
  
  if (wea.indexOf(rain) != -1) {
    symbol = RAIN;
    return;
  }

  if (wea.indexOf(thunder) != -1) {
    symbol = THUNDER;
    return;
  }

  if (wea.indexOf(cloud) != -1) {
    symbol = CLOUD;
    return;
  }

  symbol = SUN;
}

void readWea(WiFiClient client) {
  client.readStringUntil('\n');
  client.readStringUntil('\n');
  String line = client.readStringUntil('\n');
  Serial.println(line);
  wea = line.substring(line.indexOf(weaStart) + 10, line.indexOf(weaEnd)); 
  client.readStringUntil('\n');
  line = client.readStringUntil('\n');
  Serial.println(line);
  high = line.substring(line.indexOf(highStart) + 6, line.indexOf(highEnd));
  low = line.substring(line.indexOf(lowStart) + 3, line.indexOf(lowEnd) + 1);    
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

  getSymbol(wea);

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
  sprintf(ld, "%d-%02d-%02d", (timeinfo.tm_year + 1900), (timeinfo.tm_mon + 1), timeinfo.tm_mday);
  localDate = ld;

  char *lt = (char *) malloc(8);
  sprintf(lt, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  localTime = lt;

  return 1;
}

void connectWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
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

void drawLocalTime(int x) {
  initLocalTime();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_crox3h_tr);
    u8g2.drawStr(20 + x, 20, localDate);
    u8g2.setFont(u8g2_font_lubB14_tr);
    u8g2.drawStr(20 + x, 47, localTime);
    u8g2.drawFrame(0, 0, 128, 64);
  } while (u8g2.nextPage());
}

void updteWeather() {
  if (minute == 0 && flag == 0) {
    Serial.println("update weather");
    connectWiFi();
    getWeather();
    closeWiFi();
    flag = 1;
  }
  if (minute != 0) {
    flag = 0;
  }
}

void setup() {
  Serial.begin(115200);

  u8g2.begin();
  u8g2.enableUTF8Print();
  
  int t = 0;
  do {
    connectWiFi();
  
    configTime(gmtOffset_sec, 0, ntpServer);
    delay(200);
    t = initLocalTime();
  } while (t == 0);

  getWeather();

  closeWiFi();
}

int frame = 0;
int dr = 0;
int dr_t = 0;
void loop() {
  delay(1000);
  
  if (frame++ <= 10) {
    if (dr_t == 0) {
      for(int x = 80; x >= 0; x-=5) {
        drawLocalTime(x);
      }
      dr_t = 1;
    } else {
      drawLocalTime(0);
    }
  
    dr = 0;
  } else {
    if (dr == 0) {
      for(int x = 80; x > 0; x-=5) {
        draw(wea.c_str(), atoi(low.c_str()), atoi(high.c_str()), x);
      }
      dr = 1;
    }
  }
  if (frame % 14 == 0) {
    frame = 0;
    dr_t = 0;
  }

  // 每小时更新天气
  updteWeather();
}
