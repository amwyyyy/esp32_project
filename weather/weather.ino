#include <WiFi.h>
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

const char* ssid     = "xiongda";
const char* password = "15999554794";

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

void drawWeatherSymbol(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
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

void drawWeather(uint8_t symbol, int degree, int degreeHigh)
{
  drawWeatherSymbol(0, 48, symbol);
  u8g2.setFont(u8g2_font_logisoso16_tf);
  u8g2.setCursor(48+3, 32);
  u8g2.print(degree);
  u8g2.print("~");
  u8g2.print(degreeHigh);
  u8g2.print("°C");
}

void draw(const char *s, uint8_t symbol, int degree, int degreeHigh)
{
  int16_t len = strlen(s);
  for(;;)
  {
    u8g2.firstPage();
    do {
      drawWeather(symbol, degree, degreeHigh);
      u8g2.setFont(u8g2_font_wqy12_t_gb2312);
      u8g2.drawUTF8((60 - 2*len), 62, s);
    } while ( u8g2.nextPage() );
  }
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
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void readWea(WiFiClient client) {
  client.readStringUntil('\n');
  client.readStringUntil('\n');
  String line = client.readStringUntil('\n');
  wea = line.substring(line.indexOf(weaStart) + 10, line.indexOf(weaEnd)); 
  client.readStringUntil('\n');
  line = client.readStringUntil('\n');
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

  Serial.println();
  Serial.println("closing connection");
}

void closeWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println();
  Serial.println("closing wifi");
}

uint8_t getSymbol(String wea) {
  String rain = "雨";
  String cloud = "云";
  String thunder = "雷";
  
  if (wea.indexOf(rain) != -1) {
    return RAIN;
  }

  if (wea.indexOf(thunder) != -1) {
    return THUNDER;
  }

  if (wea.indexOf(cloud) != -1) {
    return CLOUD;
  }

  return SUN;
}

void setup() {
  Serial.begin(115200);
  delay(10);
  connectWiFi();
  getWeather();
  u8g2.begin();
  u8g2.enableUTF8Print();
}

void loop() {
  uint8_t symbol = getSymbol(wea);
  draw(wea.c_str(), symbol, atoi(low.c_str()), atoi(high.c_str()));
  delay(10000);
}
