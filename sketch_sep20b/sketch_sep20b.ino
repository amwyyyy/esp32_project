#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();
}

void loop() {
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_4x6_tf);
    u8g2.setCursor(0, 15);
    u8g2.print("Hello World!");
    u8g2.setFont(u8g2_font_baby_tf);
    u8g2.setCursor(0, 30);
    u8g2.print("Hello World!");
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.setCursor(0, 45);
    u8g2.print("Hello World!");
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(0, 60);
    u8g2.print("Hello World!");
  } while ( u8g2.nextPage() );
  delay(1000);
}
