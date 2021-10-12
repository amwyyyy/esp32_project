#include <WiFi.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Preferences.h>
#include <TJpg_Decoder.h>
#include <time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// 字体
#include "font/ZdyLwFont_20.h"
#include "font/FxLED_32.h"

// 图片资源
#include "img/start_gif.h"
#include "img/weather_code_jpg.h"
#include "img/temperature.h"
#include "img/humidity.h"
#include "img/Gif/taikongren.h"
#include "img/setWiFi_img.h"

#define TFT_BL 22

Preferences preferences; 
String ssid, pwd, cityCode;
#include "src/SetWiFi.h"

TFT_eSPI tft     = TFT_eSPI();  
TFT_eSprite clk  = TFT_eSprite(&tft);

// WiFi 连接计数
int connectTimes = 0;
byte loadNum     = 6;

// ntp 时间配置
const char* ntpServer1    = "ntp.aliyun.com";
const char* ntpServer2    = "cn.ntp.org.cn";
const char* ntpServer3    = "cn.pool.ntp.org";
const long gmtOffset_sec  = 3600 * 8;

// flag
bool ntpConfigFlag  = false;
bool weatherFlag    = false;
bool setWiFiFlag   = false;
const int CREATE    = 1;
const int UN_CREATE = 0;

// 天气相关
String wendu = "", shidu = "";
String scrollText[6]; // 滚动显示天气概况
String suggestScrollText[9]; // 滚动显示天气建议

struct {
  int year;
  int month;
  int day;
  int week;
  int hour;
  int minute;
  int second;
} nowTime;

struct {
  int Warn_Number1;
  int Warn_Number2;
  int Warn_Value1;
  int Warn_Value2;
  int Warn_Flag;
} warn;

// TFT 显示回调方法
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if (y >= tft.height()) {
    return 0;
  }

  tft.pushImage(x, y, w, h, bitmap);
  return 1;
}

// WiFi 连接 loading 动画
void loading(byte delayTime, byte NUM){
  loadNum += NUM;
  if(loadNum > 192){
    loadNum = 192;
  }

  clk.setColorDepth(8);
  clk.createSprite(200, 50);
  clk.fillSprite(TFT_BLACK);
  clk.loadFont(ZdyLwFont_20);
  clk.drawRoundRect(0, 0, 200, 16, 8, TFT_WHITE);
  clk.fillRoundRect(4, 3, loadNum, 10, 5, TFT_WHITE);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, TFT_BLACK); 
  clk.drawString("正在连接 " + ssid + "...", 100, 40, 2);
  clk.pushSprite(20, 110);
  clk.deleteSprite();
  clk.unloadFont();
  delay(delayTime);
}

// 显示wifi连接失败，并重新进入配网模式
void displayConnectWifiFalse() {
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
  TJpgDec.drawJpg(0, 0, wififalse, sizeof(wififalse)); 
  vTaskDelay(5000);
}

// 获取当前时间
int getNowTime(){ 
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return 0;
  }

  nowTime.year = timeinfo.tm_year + 1900;
  nowTime.month = timeinfo.tm_mon + 1;
  nowTime.day = timeinfo.tm_mday;
  nowTime.week = timeinfo.tm_wday;
  nowTime.hour = timeinfo.tm_hour;
  nowTime.minute = timeinfo.tm_min;
  nowTime.second = timeinfo.tm_sec;

  return 1;
}

void drawGrid() {
  tft.fillScreen(TFT_WHITE);

  //实心矩形
  tft.fillRoundRect(0, 0, 240, 240, 0, TFT_WHITE);

  //绘制线框
  tft.drawFastHLine(0, 0, 240, TFT_BLACK);
  tft.drawFastHLine(0, 34, 240, TFT_BLACK);
  tft.drawFastHLine(0, 200, 240, TFT_BLACK);
  tft.drawFastVLine(150, 0, 34, TFT_BLACK);
  tft.drawFastHLine(0, 166, 240, TFT_BLACK);
  tft.drawFastVLine(60, 166, 34, TFT_BLACK);
  tft.drawFastVLine(160, 166, 34, TFT_BLACK);
}

void drawCityName(JsonObject sk) {
  clk.setColorDepth(8);
  clk.loadFont(ZdyLwFont_20);
  clk.createSprite(88, 32);
  clk.fillSprite(TFT_WHITE);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, TFT_WHITE); 
  clk.drawString(sk["cityname"].as<String>(), 44, 18);
  clk.pushSprite(151, 1);
  clk.deleteSprite();
  clk.unloadFont();
}

void drawPM25(JsonObject sk) {
  uint16_t pm25BgColor;
  String aqiTxt;
  int pm25V = sk["aqi"];

  if(pm25V >= 301) {
    pm25BgColor = tft.color565(255, 36, 0);
    aqiTxt = "严重";
  } else if(pm25V >= 201 & pm25V <= 300){
    pm25BgColor = tft.color565(136, 11, 32);
    aqiTxt = "重度";
  } else if(pm25V >= 151 & pm25V <= 200){
    pm25BgColor = tft.color565(186, 55, 121);
    aqiTxt = "中度";
  } else if(pm25V >= 101 & pm25V <= 160){
    pm25BgColor = tft.color565(242, 159, 57);
    aqiTxt = "轻度";
  } else if(pm25V >= 51 & pm25V <= 100){
    pm25BgColor = tft.color565(247, 219, 100);
    aqiTxt = "良";
  } else if(pm25V >= 0 & pm25V <= 50) {
    pm25BgColor = tft.color565(156, 202, 127);
    aqiTxt = "优";
  }

  clk.setColorDepth(8);
  clk.loadFont(ZdyLwFont_20);
  clk.createSprite(50, 24); 
  clk.fillSprite(TFT_WHITE);
  clk.fillRoundRect(0, 0, 50, 24, 4, pm25BgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE); 
  clk.drawString(aqiTxt, 25, 14);
  clk.pushSprite(5, 140);
  clk.deleteSprite();
  clk.unloadFont();
}

void drawWeatherIcon(JsonObject dz) {
  String weatherCodeText = dz["weathercode"].as<String>();
  int weatherCode = weatherCodeText.substring(1, weatherCodeText.length() + 1).toInt();

  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);

  switch(weatherCode) {
    case 0: TJpgDec.drawJpg(10, 105, d00_40X30, sizeof(d00_40X30));break;
    case 1: TJpgDec.drawJpg(10, 105, d01_40X30, sizeof(d01_40X30));break;
    case 2: TJpgDec.drawJpg(10, 105, d02_40X30, sizeof(d02_40X30));break;
    case 3: TJpgDec.drawJpg(10, 105, d03_40X30, sizeof(d03_40X30));break;
    case 4: TJpgDec.drawJpg(10, 105, d04_40X30, sizeof(d04_40X30));break;
    case 5: TJpgDec.drawJpg(10, 105, d05_40X30, sizeof(d05_40X30));break;
    case 6: TJpgDec.drawJpg(10, 105, d06_40X30, sizeof(d06_40X30));break;
    case 7: TJpgDec.drawJpg(10, 105, d07_40X30, sizeof(d07_40X30));break;
    case 8: TJpgDec.drawJpg(10, 105, d08_40X30, sizeof(d08_40X30));break;
    case 9: TJpgDec.drawJpg(10, 105, d09_40X30, sizeof(d09_40X30));break;
    case 10: TJpgDec.drawJpg(10, 105, d10_40X30, sizeof(d10_40X30));break;
    case 11: TJpgDec.drawJpg(10, 105, d11_40X30, sizeof(d11_40X30));break;
    case 12: TJpgDec.drawJpg(10, 105, d12_40X30, sizeof(d12_40X30));break;
    case 13: TJpgDec.drawJpg(10, 105, d13_40X30, sizeof(d13_40X30));break;
    case 14: TJpgDec.drawJpg(10, 105, d14_40X30, sizeof(d14_40X30));break;
    case 15: TJpgDec.drawJpg(10, 105, d15_40X30, sizeof(d15_40X30));break;
    case 16: TJpgDec.drawJpg(10, 105, d16_40X30, sizeof(d16_40X30));break;
    case 17: TJpgDec.drawJpg(10, 105, d17_40X30, sizeof(d17_40X30));break;
    case 18: TJpgDec.drawJpg(10, 105, d18_40X30, sizeof(d18_40X30));break;
    case 19: TJpgDec.drawJpg(10, 105, d19_40X30, sizeof(d19_40X30));break;
    case 20: TJpgDec.drawJpg(10, 105, d20_40X30, sizeof(d20_40X30));break;
    case 21: TJpgDec.drawJpg(10, 105, d21_40X30, sizeof(d21_40X30));break;
    case 22: TJpgDec.drawJpg(10, 105, d22_40X30, sizeof(d22_40X30));break;
    case 23: TJpgDec.drawJpg(10, 105, d23_40X30, sizeof(d23_40X30));break;
    case 24: TJpgDec.drawJpg(10, 105, d24_40X30, sizeof(d24_40X30));break;
    case 25: TJpgDec.drawJpg(10, 105, d25_40X30, sizeof(d25_40X30));break;
    case 26: TJpgDec.drawJpg(10, 105, d26_40X30, sizeof(d26_40X30));break;
    case 27: TJpgDec.drawJpg(10, 105, d27_40X30, sizeof(d27_40X30));break;
    case 28: TJpgDec.drawJpg(10, 105, d28_40X30, sizeof(d28_40X30));break;
    case 29: TJpgDec.drawJpg(10, 105, d29_40X30, sizeof(d29_40X30));break;
    case 30: TJpgDec.drawJpg(10, 105, d30_40X30, sizeof(d30_40X30));break;
    case 31: TJpgDec.drawJpg(10, 105, d31_40X30, sizeof(d31_40X30));break;
    case 32: TJpgDec.drawJpg(10, 105, d32_40X30, sizeof(d32_40X30));break;
    case 33: TJpgDec.drawJpg(10, 105, d33_40X30, sizeof(d33_40X30));break;
    case 49: TJpgDec.drawJpg(10, 105, d49_40X30, sizeof(d49_40X30));break;
    case 53: TJpgDec.drawJpg(10, 105, d53_40X30, sizeof(d53_40X30));break;
    case 54: TJpgDec.drawJpg(10, 105, d54_40X30, sizeof(d54_40X30));break;
    case 55: TJpgDec.drawJpg(10, 105, d55_40X30, sizeof(d55_40X30));break;
    case 56: TJpgDec.drawJpg(10, 105, d56_40X30, sizeof(d56_40X30));break;
    case 57: TJpgDec.drawJpg(10, 105, d57_40X30, sizeof(d57_40X30));break;
    case 58: TJpgDec.drawJpg(10, 105, d58_40X30, sizeof(d58_40X30));break;
    case 301: TJpgDec.drawJpg(10, 105, d301_40X30, sizeof(d301_40X30));break;
    case 302: TJpgDec.drawJpg(10, 105, d302_40X30, sizeof(d302_40X30));break;
    default:break;
  }
}

void weatherSuggest(JsonObject dataSuggestJson) {
  // 路况
  suggestScrollText[0] = dataSuggestJson["lk_name"].as<String>() + " " + dataSuggestJson["lk_hint"].as<String>();
  // 晨练
  suggestScrollText[1] = dataSuggestJson["cl_name"].as<String>() + " " + dataSuggestJson["cl_hint"].as<String>();
  // 紫外线
  suggestScrollText[2] = dataSuggestJson["uv_name"].as<String>() + " " + dataSuggestJson["uv_hint"].as<String>();
  // 穿衣
  suggestScrollText[3] = dataSuggestJson["ct_name"].as<String>() + " " + dataSuggestJson["ct_hint"].as<String>();
  // 感冒
  suggestScrollText[4] = dataSuggestJson["gm_name"].as<String>() + " " + dataSuggestJson["gm_hint"].as<String>();
  // 雨伞
  suggestScrollText[5] = dataSuggestJson["ys_name"].as<String>() + " " + dataSuggestJson["ys_hint"].as<String>();
  // 干燥
  suggestScrollText[6] = dataSuggestJson["gz_name"].as<String>() + " " + dataSuggestJson["gz_hint"].as<String>();
  // 空调
  suggestScrollText[7] = dataSuggestJson["ac_name"].as<String>() + " " + dataSuggestJson["ac_hint"].as<String>();
  // 舒适度
  suggestScrollText[8] = dataSuggestJson["co_name"].as<String>() + " " + dataSuggestJson["co_hint"].as<String>();
}

// 天气信息写到屏幕上
void drawWeaterData(String *cityDZ, String *dataSK, String *dataFC, String *dataSuggest, String *dataWarn1) {
  // 解析 json 字符串
  DynamicJsonDocument doc(8192);
  deserializeJson(doc, *dataSK);
  JsonObject sk = doc.as<JsonObject>();

  // 获取温度湿度
  wendu = sk["temp"].as<String>();
  shidu = sk["SD"].as<String>();

  //城市名称
  drawCityName(sk);

  //PM2.5空气指数
  drawPM25(sk);

  //左上角滚动字幕
  scrollText[0] = "实时天气 " + sk["weather"].as<String>();
  scrollText[1] = "风向 " + sk["WD"].as<String>() + sk["WS"].as<String>();
  scrollText[2] = "PM2.5 " + sk["aqi_pm25"].as<String>();

  deserializeJson(doc, *cityDZ);
  JsonObject dz = doc.as<JsonObject>();
  scrollText[3] = "今日 " + dz["weather"].as<String>();
  //  天气图标
  drawWeatherIcon(dz);

  deserializeJson(doc, *dataFC);
  JsonObject fc = doc.as<JsonObject>();
  scrollText[4] = "最低温度 " + fc["fd"].as<String>() + "℃";
  scrollText[5] = "最高温度 " + fc["fc"].as<String>() + "℃";

  // 天气建议
  deserializeJson(doc, *dataSuggest);
  JsonObject dataSuggestJson = doc.as<JsonObject>();
  weatherSuggest(dataSuggestJson);
}

// 星期
String week() {
  String wk[7] = {"日", "一", "二", "三", "四", "五", "六"};
  String s = "周" + wk[nowTime.week];
  return s;
}

String monthDay() {
  String s = String(nowTime.month) + "月" + String(nowTime.day) + "日";
  return s;
}

void connectWiFi() {
  Serial.println("正在连接 " + ssid + "...");
  WiFi.begin(ssid.c_str(), pwd.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    loading(100, 1);
    connectTimes++;
    if(connectTimes >= 190) {
      displayConnectWifiFalse();
      preferences.clear();
      ESP.restart();
    }
  }

  Serial.print("本地IP： ");
  Serial.println(WiFi.localIP());

  // 让动画走完
  while(loadNum < 192 & connectTimes < 190){
    loading(0, 5);
  }
}

void quickConnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("正在连接 " + ssid + "...");
    WiFi.begin(ssid.c_str(), pwd.c_str());

    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
      count++;
      if (count > 100) {
        break;
      }
      vTaskDelay(200);
    }

    Serial.print("本地IP： ");
    Serial.println(WiFi.localIP());
  }
}

void closeWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println();
  Serial.println("closing wifi");
}

void displayTime(char *hm, char *sec) {
  clk.setColorDepth(8);

  // 时分
  clk.createSprite(140, 48);
  clk.fillSprite(TFT_WHITE);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, TFT_WHITE);
  clk.drawString(hm, 70, 24, 7);
  clk.pushSprite(28, 40);
  clk.deleteSprite();

  // 秒
  clk.createSprite(40, 28);
  clk.fillSprite(TFT_WHITE);
  clk.loadFont(FxLED_32);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, TFT_WHITE); 
  clk.drawString(sec, 20, 12);
  clk.pushSprite(170, 55);
  clk.deleteSprite();
  clk.unloadFont();

  //星期
  clk.loadFont(ZdyLwFont_20);
  clk.createSprite(58, 32);
  clk.fillSprite(TFT_WHITE);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, TFT_WHITE);
  clk.drawString(week(), 29, 16);
  clk.pushSprite(1, 168);
  clk.deleteSprite();

  //月日
  clk.createSprite(98, 32);
  clk.fillSprite(TFT_WHITE);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, TFT_WHITE);  
  clk.drawString(monthDay(), 49, 16);
  clk.pushSprite(61, 168);
  clk.deleteSprite();
  clk.unloadFont();
}

void displayLeftScroll(String info) {
  if (scrollText->length() > 0) {
    clk.loadFont(ZdyLwFont_20);
    for(int pos = 20; pos > 0; pos--) {
      clk.createSprite(148, 24); 
      clk.fillSprite(TFT_WHITE);
      clk.setTextWrap(false);
      clk.setTextDatum(CC_DATUM);
      clk.setTextColor(TFT_BLACK, TFT_WHITE); 
      clk.drawString(info, 74, pos + 14);
      clk.pushSprite(2, 4);
      clk.deleteSprite();
    }
    clk.unloadFont();
  }
}

void displayBottomScroll(String info) {
  if (suggestScrollText->length() > 0) {
    clk.loadFont(ZdyLwFont_20);
    clk.createSprite(240, 40); 
    clk.fillSprite(TFT_WHITE);
    clk.setTextDatum(CC_DATUM);
    clk.setTextColor(TFT_BLACK, TFT_WHITE); 
    clk.drawString(info, 120, 20);
    clk.pushSprite(0, 201);
    clk.deleteSprite();
    clk.unloadFont();
  }
}

void displayWsd(int wsd_index) {
  clk.loadFont(ZdyLwFont_20);

  if (wsd_index == 0) {
    TJpgDec.drawJpg(165, 171, temperature, sizeof(temperature));
    for(int pos = 20; pos > 0; pos--) {
      clk.createSprite(50, 32); 
      clk.fillSprite(TFT_WHITE);
      clk.setTextDatum(CC_DATUM);
      clk.setTextColor(TFT_BLACK, TFT_WHITE); 
      clk.drawString(wendu + "℃", 25, pos + 16);
      clk.pushSprite(188, 168);
    }
  } else {
    TJpgDec.drawJpg(165, 171, humidity, sizeof(humidity));
    for(int pos = 20; pos > 0; pos-=2) {
      clk.createSprite(50, 32); 
      clk.fillSprite(TFT_WHITE);
      clk.setTextDatum(CC_DATUM);
      clk.setTextColor(TFT_BLACK, TFT_WHITE);   
      clk.drawString(shidu, 25, pos + 16);
      clk.pushSprite(188, 168);
    }
  }
  clk.deleteSprite();
  clk.unloadFont();
}

void displayImage(int imgNum, int x, int y) {  
  switch(imgNum) {
    case 1: TJpgDec.drawJpg(x, y, i0, sizeof(i0));break;
    case 2: TJpgDec.drawJpg(x, y, i1, sizeof(i1));break;
    case 3: TJpgDec.drawJpg(x, y, i2, sizeof(i2));break;
    case 4: TJpgDec.drawJpg(x, y, i3, sizeof(i3));break;
    case 5: TJpgDec.drawJpg(x, y, i4, sizeof(i4));break;
    case 6: TJpgDec.drawJpg(x, y, i5, sizeof(i5));break;
    case 7: TJpgDec.drawJpg(x, y, i6, sizeof(i6));break;
    case 8: TJpgDec.drawJpg(x, y, i7, sizeof(i7));break;
    case 9: TJpgDec.drawJpg(x, y, i8, sizeof(i8));break;
    case 10: TJpgDec.drawJpg(x, y, i9, sizeof(i9));break;
  }
}

void setWiFi() {
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
  TJpgDec.drawJpg(0, 0, setWiFi_img, sizeof(setWiFi_img));

  initBasic();
  initSoftAP();
  initWebServer();
  initDNS();
  while(setWiFiFlag == false) {
    server.handleClient();
    dnsServer.processNextRequest();
    if(WiFi.status() == WL_CONNECTED) {
      server.stop();
      setWiFiFlag = true;
    }
  }
}

void TaskNtpTime(void *pvParameters) {
  int *create = (int *) pvParameters;

  for(;;) {
    int t = 0;
    do {
      configTime(gmtOffset_sec, 0, ntpServer1, ntpServer2, ntpServer3);
      vTaskDelay(200);
      t = getNowTime();
    } while (t == 0);
    Serial.println("config ntp time finish!");
    
    if (*create == 1) {
      xTaskCreatePinnedToCore(TaskDisplay, "TaskDisplay", 1024 * 3, NULL, 3, NULL, 1);
    }

    ntpConfigFlag = true;
    vTaskDelete(NULL);
  }
}

void TaskGetCityCode(void *pvParameters) {
  //创建 HTTPClient 对象
  HTTPClient httpClient;

  int count = 0;
  while (cityCode == "none") {
    String URL = "http://wgeo.weather.com.cn/ip/";
    httpClient.begin(URL);

    //设置请求头中的User-Agent
    httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
    httpClient.addHeader("Referer", "http://www.weather.com.cn/");

    //启动连接并发送HTTP请求
    int httpCode = httpClient.GET();
    Serial.println("数据请求中...");

    vTaskDelay(200);

    //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    if (httpCode == HTTP_CODE_OK) {
      String str = httpClient.getString();
      int id = str.indexOf("id=");
      if(id > -1) {
        String cc = str.substring(id + 4, id + 4 + 9);
        cityCode = cc;
        Serial.println("获取城市代码成功, " + cityCode);
      } else {
        Serial.println("获取城市代码失败，正在重新获取...");  
      }
    } else {
      Serial.print("请求城市代码错误：");
      Serial.println(String(httpCode) + " 正在重新获取...");
      vTaskDelay(1000);
    }

    // 重试两次
    if (count++ > 2) {
      Serial.println("获取城市代码失败，重启!");  
      ESP.restart();
    }

    vTaskDelay(50);
  }

  xTaskCreatePinnedToCore(TaskGetCityWeather, "GetCityWeather", 1024 * 3, NULL, 2, NULL, 0);

  vTaskDelete(NULL);
}

void TaskGetCityWeather(void *pvParameters) {
  HTTPClient httpClient;

  for(;;) {
    String URL = "http://d1.weather.com.cn/weather_index/" + cityCode + ".html";
    httpClient.begin(URL); 

    //设置请求头中的User-Agent
    httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
    httpClient.addHeader("Referer", "http://www.weather.com.cn/");

    //启动连接并发送HTTP请求
    int httpCode = httpClient.GET();
    Serial.println("正在获取天气数据");
    Serial.println(URL);

    vTaskDelay(200);

    if (httpCode == HTTP_CODE_OK) {
      String str = httpClient.getString();

      int indexStart = str.indexOf("weatherinfo\":");
      int indexEnd = str.indexOf("};var alarmDZ");
      String jsonCityDZ = str.substring(indexStart + 13, indexEnd);

      // 气象预警不同时间会发布不同的预警信息，只会显示最新的一个
      indexStart = str.indexOf("alarmDZ ={\"w\":[");
      indexEnd = str.indexOf("]};var dataSK");
      String jsonDataWarn1 = str.substring(indexStart + 15, indexEnd);
      if(jsonDataWarn1.length() >= 40) {
        warn.Warn_Flag = 1;
      } else {
        warn.Warn_Flag = 0;
      }

      indexStart = str.indexOf("dataSK =");
      indexEnd = str.indexOf(";var dataZS");
      String jsonDataSK = str.substring(indexStart + 8,indexEnd);

      indexStart = str.indexOf("\"f\":[");
      indexEnd = str.indexOf(",{\"fa");
      String jsonFC = str.substring(indexStart + 5,indexEnd);

      indexStart = str.indexOf(";var dataZS ={\"zs\":");
      indexEnd = str.indexOf(",\"cn\":\"长沙\"};var fc =");
      String jsonSuggest = str.substring(indexStart + 19, indexEnd);
      
      drawWeaterData(&jsonCityDZ, &jsonDataSK, &jsonFC, &jsonSuggest, &jsonDataWarn1);
      Serial.println("天气数据获取成功");
      httpClient.end();

      weatherFlag = true;
      vTaskDelete(NULL);
    } else {
      Serial.print("请求城市天气错误：");
      Serial.println(String(httpCode) + " 正在重新获取...");
    }
    
    vTaskDelay(50);
  }
}

void TaskDisplay(void *pvParameters) {
  long dt = 0;
  char *hm = (char *) malloc(6);
  char *sec = (char *) malloc(3);

  long top = 0, bottom = 0, wsd = 0;
  int top_index = 0, bottom_index = 0, wsd_index = 0;

  int imgNum = 1;
  long tImg = 0;

  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);

  for (;;) {
    long now = millis();

    if (now - dt > 200) {
      if (getNowTime() != 0) {
        sprintf(hm, "%02d:%02d", nowTime.hour, nowTime.minute);
        sprintf(sec, "%02d", nowTime.second);
        displayTime(hm, sec);
      }
      dt = millis();
    }

    // 显示动画
    if (now - tImg > 100) {
      displayImage(imgNum, 100, 94);
      if (imgNum++ > 9) {
        imgNum = 1;
      }
      tImg = millis();
    }

    // 左上角滚动信息
    if (now - top > 3500) {
      displayLeftScroll(scrollText[top_index++]);
      top = millis();
      if (top_index > 5) {
        top_index = 0;
      }
    }

    // 底部滚动信息
    if (now - bottom > 5000) {
      displayBottomScroll(suggestScrollText[bottom_index++]);
      bottom = millis();
      if (bottom_index > 8) {
        bottom_index = 0;
      }
    }

    // 显示温湿度
    if (now - wsd > 6000) {
      displayWsd(wsd_index);
      if (wsd_index++ > 0) {
        wsd_index = 0;
      }
      wsd = millis();
    }

    vTaskDelay(10);
  }
}

void TaskUpdateData(void *pvParameters) {
  int updateWeather = 0;
  int updateNtp = 0;

  for (;;) {
    updateWeather++;
    if (updateWeather == 60) {
      quickConnectWiFi();
      weatherFlag = false;
      xTaskCreatePinnedToCore(TaskGetCityWeather, "GetCityWeather", 1024 * 3, NULL, 2, NULL, 1);
      updateWeather = 0;
    }

    updateNtp++;
    if (updateNtp == 120) {
      quickConnectWiFi();
      ntpConfigFlag = false;
      xTaskCreatePinnedToCore(TaskNtpTime, "NtpTime", 1024 * 2, (void *) &UN_CREATE, 3, NULL, 0);
      updateNtp = 0;
    }

    vTaskDelay(60 * 1000);
  }
}

void TaskDisconnectWiFi(void *pvParameters) {
  for (;;) {
    if (weatherFlag && ntpConfigFlag) {
      if (WiFi.status() == WL_CONNECTED) {
        closeWiFi();
      }
    }

    vTaskDelay(2 * 60 * 1000);
  }
}

void setup() {
  Serial.begin(115200);

  // 初始化显示屏
  tft.init();
  tft.setRotation(3);

  // 设置显示屏背光
  ledcSetup(0, 5000, 8);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, 60);

  tft.fillScreen(TFT_BLACK);

  //首次使用自动进入配网模式, 读取 NVS 存储空间内的 ssid、password 和 citycode
  preferences.begin("wifi", false);
  ssid =  preferences.getString("ssid", "none"); 
  pwd =  preferences.getString("password", "none");
  // cityCode = preferences.getString("citycode", "none");
  cityCode = "101280601";
  preferences.end();

  if(ssid == "none") {
    // 配网
    setWiFi();
  }

  // 连接WiFi
  connectWiFi();

  // 绘制网格
  drawGrid();

  // 连接WiFi后同步网络时间
  xTaskCreatePinnedToCore(TaskNtpTime, "NtpTime", 1024 * 2, (void *) &CREATE, 3, NULL, 0);
  // 获取城市编码
  xTaskCreatePinnedToCore(TaskGetCityCode, "GetCityCode", 1024 * 4, NULL, 2, NULL, 0);
  // 定时更新数据
  xTaskCreatePinnedToCore(TaskUpdateData, "UpdateData", 1024 * 2, NULL, 1, NULL, 0);
  // 定时端口闲置 WiFI
  xTaskCreatePinnedToCore(TaskDisconnectWiFi, "DisconnectWiFi", 1024 * 2, NULL, 1, NULL, 0);
}

void loop() {

}
