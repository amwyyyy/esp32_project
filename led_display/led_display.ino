#include <SPI.h>
#include "LedMatrix.h"
#define NUMBER_OF_DEVICES 4 //串联led矩阵连接数
#define CS_PIN 15
#define CLK_PIN 14
#define MISO_PIN 2 //我们不使用此引脚只需填写匹配构造函数
#define MOSI_PIN 12
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  
void setup() {
  ledMatrix.init();
  ledMatrix.setText("Hello World!");
}
void loop() {
  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();
  delay(50);
}
