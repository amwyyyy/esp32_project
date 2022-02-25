#include <SPI.h>
#include <time.h>
#include <WiFi.h>
#include "LedMatrix.h"
#include "cp437font.h"

#define NUMBER_OF_DEVICES 4 //串联led矩阵连接数
#define CS_PIN 5
#define CLK_PIN 18
#define MISO_PIN 2 //我们不使用此引脚只需填写匹配构造函数
#define MOSI_PIN 23
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

const char* ntpServer    = "ntp.aliyun.com";
const long gmtOffset_sec = 3600 * 8;

String ssid = "xiongda";
String password = "15999554794";

// 定时更新时间
void TaskUpdateData(void *pvParameters);

struct {
  int hour;
  int minute;
  int second;
} now;
  
void connectWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid.c_str(), password.c_str());

  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
      delay(300);
      if (count++ > 200) {
        ESP.restart();
      }
  }

  Serial.print("WiFi connected. ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

int initLocalTime(){ 
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 0;
  }

  now.hour = timeinfo.tm_hour;
  now.minute = timeinfo.tm_min;
  now.second = timeinfo.tm_sec;

  return 1;
}

void closeWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println();
  Serial.println("closing wifi");
}

void TaskUpdateData(void *pvParameters) {
  for (;;) {
    connectWiFi();

    int t = 0;
    do {
      configTime(gmtOffset_sec, 0, ntpServer);
      vTaskDelay(200);
      t = initLocalTime();
    } while (t == 0);
    Serial.println("config time finish!");

    closeWiFi();

    // 每小时更新一次
    vTaskDelay(60 * 60 * 1000);
  }
}

void setup() {
  Serial.begin(115200);
  ledMatrix.init();

  xTaskCreatePinnedToCore(TaskUpdateData, "TaskUpdateData", 1024 * 4, NULL, 1, NULL, 1);
}

char *lt = (char *) malloc(4);
bool flag = true;

void loop() {
  ledMatrix.clear();

  initLocalTime();
  sprintf(lt, "%02d%02d", now.hour, now.minute);
  String text = lt;
  for (int i = 0; i < text.length(); i++) {
    char ch = text.charAt(i);
    
    for (int n = 0; n < 8; n++) {
      int pos = n + i * 8;
      ledMatrix.setColumn(pos, pgm_read_byte(&cp437_font [ch] [n]));
    }
  }

  if (flag) {
    ledMatrix.setPixel(15, 2);
    ledMatrix.setPixel(15, 4);
  }
  flag = !flag;

  ledMatrix.commit();
  delay(1000);
}
