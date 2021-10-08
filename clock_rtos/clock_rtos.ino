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

#define TFT_BL 22

Preferences preferences; 
String ssid, pwd, cityCode;

TFT_eSPI tft    = TFT_eSPI();  
TFT_eSprite clk = TFT_eSprite(&tft);

// WiFi 连接计数
int connectTimes = 0;
byte loadNum     = 6;

// ntp 时间配置
const char* ntpServer1    = "ntp.aliyun.com";
const char* ntpServer2    = "cn.ntp.org.cn";
const char* ntpServer3    = "cn.pool.ntp.org";
const long gmtOffset_sec  = 3600 * 8;

// 天气相关
String wendu = "", shidu = "";
String scrollText[6]; // 滚动显示文字

struct {
  int year;
  int month;
  int day;
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

// 任务句柄
TaskHandle_t xHandle_loading = NULL;
TaskHandle_t xHandle_ntp_time = NULL;
TaskHandle_t xHandle_get_weather = NULL;

/** 定义任务 */
// 连接 WiFi
void TaskConnectWiFi(void *pvParameters);
// WiFi 连接进度条
void TaskLoading(void *pvParameters);
// 同步时间
void TaskNtpTime(void *pvParameters);
// 显示时间
void TaskDisplayTime(void *pvParameters);
// 获取城市编码
void TaskGetCityCode(void *pvParameters);
// 获取城市天气
void TaskGetCityWeather(void *pvParameters);
// 左上角信息滚动显示
void TaskLeftScroll(void *pvParameters);

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
  vTaskDelay(delayTime);
  clk.unloadFont(); //释放加载字体资源
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
  clk.createSprite(88, 32);
  clk.fillSprite(TFT_WHITE);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, TFT_WHITE); 
  clk.drawString(sk["cityname"].as<String>(), 44, 18);
  clk.pushSprite(151, 1);
  clk.deleteSprite();
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

  clk.createSprite(50, 24); 
  clk.fillSprite(TFT_WHITE);
  clk.fillRoundRect(0, 0, 50, 24, 4, pm25BgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE); 
  clk.drawString(aqiTxt, 25, 14);
  clk.pushSprite(5, 140);
  clk.deleteSprite();
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

  /** 绘制相关文字 **/
  clk.setColorDepth(8);
  clk.loadFont(ZdyLwFont_20);
 
  //城市名称
  drawCityName(sk);

  //PM2.5空气指数
  drawPM25(sk);

  //左上角滚动字幕
  String scrollText[6];
  scrollText[0] = "实时天气 " + sk["weather"].as<String>();
  // scrollText[1] = "空气质量 " + aqiTxt;
  scrollText[2] = "风向 " + sk["WD"].as<String>() + sk["WS"].as<String>();
  scrollText[6] = "PM2.5 " + sk["aqi_pm25"].as<String>();
  
  deserializeJson(doc, *cityDZ);
  JsonObject dz = doc.as<JsonObject>();
  scrollText[3] = "今日 " + dz["weather"].as<String>();

  // String weatherCodeText = dz["weathercode"].as<String>();
  // weatherCode = weatherCodeText.substring(1, weatherCodeText.length() + 1).toInt();

  deserializeJson(doc, *dataFC);
  JsonObject fc = doc.as<JsonObject>();
  scrollText[4] = "最低温度 " + fc["fd"].as<String>() + "℃";
  scrollText[5] = "最高温度 " + fc["fc"].as<String>() + "℃";

  xTaskCreatePinnedToCore(TaskLeftScroll, "TaskLeftScroll", 1024 * 4, (void *) scrollText, 4, NULL, ARDUINO_RUNNING_CORE);
}

void TaskConnectWiFi(void *pvParameters) {
  Serial.println("正在连接 " + ssid + "...");
  WiFi.begin(ssid.c_str(), pwd.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(100);
    connectTimes++;
    if(connectTimes >= 190) {
      vTaskDelete(xHandle_loading);
      displayConnectWifiFalse();
      preferences.clear();
      ESP.restart();
    }
  }

  Serial.print("本地IP： ");
  Serial.println(WiFi.localIP());

  // 连接WiFi后同步网络时间
  xTaskCreatePinnedToCore(TaskNtpTime, "TaskNtpTime", 1024 * 2, NULL, 3, &xHandle_ntp_time, ARDUINO_RUNNING_CORE);
  // 获取城市编码
  xTaskCreatePinnedToCore(TaskGetCityCode, "TaskGetCityCode", 1024 * 4, NULL, 3, NULL, ARDUINO_RUNNING_CORE);

  vTaskDelete(NULL);
}

void TaskLoading(void *pvParameters) {
  // 显示连接动画
  while (WiFi.status() != WL_CONNECTED) {
    loading(100, 1);
  }

  // 让动画走完
  while(loadNum < 192 & connectTimes < 190){
    loading(0, 5);
  }

  // 绘制网格
  drawGrid();

  vTaskDelete(NULL);
}

void TaskNtpTime(void *pvParameters) {
  for(;;) {
    int t = 0;
    do {
      configTime(gmtOffset_sec, 0, ntpServer1, ntpServer2, ntpServer3);
      vTaskDelay(200);
      t = getNowTime();
    } while (t == 0);
    Serial.println("config ntp time finish!");

    xTaskCreatePinnedToCore(TaskDisplayTime, "TaskDisplayTime", 1024 * 4, NULL, 4, NULL, ARDUINO_RUNNING_CORE);
    vTaskSuspend(xHandle_ntp_time);
  }
}

void TaskDisplayTime(void *pvParameters) {
  // 接收格式化字符串
  char *hm = (char *) malloc(6);
  char *sec = (char *) malloc(2);

  for(;;) {
    getNowTime();
    sprintf(hm, "%02d:%02d", nowTime.hour, nowTime.minute);
    sprintf(sec, "%02d", nowTime.second);

    // 时分
    clk.setColorDepth(8);
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
    clk.unloadFont();
    clk.pushSprite(170, 55);
    clk.deleteSprite();

    vTaskDelay(250);
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
    }

    // 重试两次
    if (count++ > 2) {
      Serial.println("获取城市代码失败，重启!");  
      ESP.restart();
    }

    vTaskDelay(200);
  }

  xTaskCreatePinnedToCore(TaskGetCityWeather, "TaskGetCityWeather", 1024 * 4, NULL, 4, &xHandle_get_weather, ARDUINO_RUNNING_CORE);

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
      vTaskSuspend(xHandle_get_weather);
    } else {
      Serial.print("请求城市天气错误：");
      Serial.println(String(httpCode) + " 正在重新获取...");
    }
    
    vTaskDelay(200);
  }
}

void TaskLeftScroll(void *pvParameters) {
  String *scrollText = (String *) pvParameters;
  Serial.println(scrollText[0]);
  vTaskDelete(NULL);
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
  // preferences.begin("wifi", false);
  // ssid =  preferences.getString("ssid", "none"); 
  // pwd =  preferences.getString("password", "none");
  // cityCode = preferences.getString("citycode", "none");
  // preferences.end();
  cityCode = "none";
  ssid = "xiongda";
  pwd = "15999554794";

  if(ssid == "none") {
    // TODO 配网
  }

  xTaskCreatePinnedToCore(TaskConnectWiFi, "TaskConnectWiFi", 1024 * 2, NULL, 3, NULL, ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(TaskLoading, "TaskLoading", 1024 * 4, NULL, 4, &xHandle_loading, ARDUINO_RUNNING_CORE);
}

void loop() {

}