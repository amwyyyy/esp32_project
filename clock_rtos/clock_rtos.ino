#include <WiFi.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Preferences.h>
#include <TJpg_Decoder.h>

// 字体
#include "font/ZdyLwFont_20.h"

// 图片资源
#include "img/start_gif.h"

#define TFT_BL 22

Preferences preferences; 
String prefSSID, prefPassword, prefCityCode;

TFT_eSPI tft = TFT_eSPI();  
TFT_eSprite clk = TFT_eSprite(&tft);

int connectTimes = 0; // WiFi 连接次数

/** 定义任务 */
// 连接 WiFi
void TaskConnectWiFi(void *pvParameters);

// TFT 显示回调方法
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if (y >= tft.height()) {
    return 0;
  }

  tft.pushImage(x, y, w, h, bitmap);
  return 1;
}

// WiFi 连接 loading 动画
byte loadNum = 6;
void loading(int delayTime, byte NUM){
  clk.setColorDepth(8);
  clk.createSprite(200, 50);
  clk.fillSprite(TFT_BLACK);
  clk.loadFont(ZdyLwFont_20);
  clk.drawRoundRect(0, 0, 200, 16, 8, TFT_WHITE);
  clk.fillRoundRect(3, 3, loadNum, 10, 5, TFT_WHITE);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, TFT_BLACK); 
  clk.drawString("正在连接 " + prefSSID + " ...", 100, 40, 2);
  clk.pushSprite(20, 110);
  clk.deleteSprite();
  loadNum += NUM;
  if(loadNum >= 194){
    loadNum = 194;
  }
  vTaskDelay(delayTime);
  clk.unloadFont(); //释放加载字体资源
}

//显示wifi连接失败，并重新进入配网模式
void displayConnectWifiFalse() {
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
  TJpgDec.drawJpg(0, 0, wififalse, sizeof(wififalse)); 
  vTaskDelay(5000);
}

void TaskConnectWiFi(void *pvParameters) {
  Serial.println("正在连接 " + prefSSID + "...");
  // WiFi.begin(prefSSID.c_str(), prefPassword.c_str());
  WiFi.begin("xiongda", "15999554794");

  Serial.print("begin");
  while (WiFi.status() != WL_CONNECTED) {
    // loading(300, 1);
    Serial.print(".");
    vTaskDelay(100);
    // if (count++ > 200) {
    //   // displayConnectWifiFalse();
    //   preferences.clear();
    //   ESP.restart();
    // }

    // for(byte n=0; n<10; n++) {
    //   loading(100, 1);
    //   connectTimes++;
    //   if(connectTimes >= 190) {
    //     connectTimes = 0;
    //     displayConnectWifiFalse();
    //     preferences.clear();
    //     ESP.restart();
    //   }
    // }
  }

  // while(loadNum < 194 & connectTimes <= 189){ //让动画走完
  //   loading(0, 5);
  //   connectTimes = 0;
  // }

  Serial.print("本地IP： ");
  Serial.println(WiFi.localIP());
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
  preferences.begin("wifi", false);
  prefSSID =  preferences.getString("ssid", "none"); 
  prefPassword =  preferences.getString("password", "none");
  prefCityCode =  preferences.getString("citycode", "none");
  preferences.end();

  if(prefSSID == "none") {
    // TODO 配网
  }

  xTaskCreatePinnedToCore(TaskConnectWiFi, "TaskConnectWiFi", 1024 * 2, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
}

void loop() {

}