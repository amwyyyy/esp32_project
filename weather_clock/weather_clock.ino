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
//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);         

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

void drawWeather(uint8_t symbol, int degree, int degreeHigh) {
  drawWeatherSymbol(0, 48, symbol);
  u8g2.setFont(u8g2_font_logisoso16_tf);
  u8g2.setCursor(48+3, 32);
  u8g2.print(degree);
  u8g2.print("~");
  u8g2.print(degreeHigh);
  u8g2.print("°C");
}

void draw(const char *s, int degree, int degreeHigh){
  int16_t len = strlen(s);
  u8g2.firstPage();
  do {
    drawWeather(symbol, degree, degreeHigh);
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2.drawUTF8((60 - 2*len), 62, s);
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
  char year[10];
  itoa(timeinfo.tm_year + 1900, year, 10);
  char month[10];
  itoa(timeinfo.tm_mon + 1, month, 10);
  char day[10];
  itoa(timeinfo.tm_mday, day, 10);

  if(strlen(month) == 1) {
    char zero[10] = "0";
    strcat(zero, month);
    strcpy(month, zero);
  }
  if (strlen(day) == 1) {
    char zero[10] = "0";
    strcat(zero, day);
    strcpy(day, zero);  
  }
  Serial.print(strcat(year, "-"));
  Serial.print(strcat(month, "-"));
  Serial.print(day);
  Serial.println("");

  char *ld = (char *) malloc(strlen(year) + strlen(month) + strlen(day));
  sprintf(ld, "%s%s%s", year, month, day);
  localDate = ld;

  char hour[10];
  itoa(timeinfo.tm_hour, hour, 10);
  char min[5];
  itoa(timeinfo.tm_min, min, 10);
  char second[5];
  itoa(timeinfo.tm_sec, second, 10);
  minute = timeinfo.tm_min;

  if (strlen(hour) == 1) {
    char zero[10] = "0";
    strcat(zero, hour);
    strcpy(hour, zero); 
  }
  if (strlen(min) == 1) {
    char zero[10] = "0";
    strcat(zero, min);
    strcpy(min, zero);  
  }
  if (strlen(second) == 1) {
    char zero[10] = "0";
    strcat(zero, second);
    strcpy(second, zero);  
  }

  char *lt = (char *) malloc(strlen(hour) + strlen(min) + strlen(second) + 2);
  sprintf(lt, "%s:%s:%s", hour, min, second);
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

void drawLocalTime() {
  initLocalTime();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(10, 25, localDate);
    u8g2.drawStr(10, 45, localTime);
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
  
  int t = 0;
  do {
    connectWiFi();
  
    configTime(gmtOffset_sec, 0, ntpServer);
    delay(200);
    t = initLocalTime();
  } while (t == 0);

  getWeather();

  closeWiFi();

  u8g2.begin();
  u8g2.enableUTF8Print();
}

int f = 0;
void loop() {
  delay(1000);
  
  if (f++ <= 10) {
    drawLocalTime();
  } else {
    draw(wea.c_str(), atoi(low.c_str()), atoi(high.c_str()));
  }
  if (f % 13 == 0) {
    f = 0;
  }

  // 每小时更新天气
  updteWeather();
}
