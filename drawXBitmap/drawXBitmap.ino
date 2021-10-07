// Example sketch to demonstrate the drawing of X BitMap (XBM)
// format image onto the display.

// Information on the X BitMap (XBM) format can be found here:
// https://en.wikipedia.org/wiki/X_BitMap

// This example is part of the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

// Created by Bodmer 23/04/18

#include <TFT_eSPI.h>        // Hardware-specific library
#include <TJpg_Decoder.h>

#include "p1.h"
#include "p2.h"
#include "p4.h"

TFT_eSPI tft = TFT_eSPI();   // Invoke library

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if (y >= tft.height()) {
    return 0;
  }

  tft.pushImage(x, y, w, h, bitmap);
  return 1;
}

void setup()
{
  tft.begin();               // Initialise the display
  tft.fillScreen(TFT_BLACK); // Black screen fill
  tft.setRotation(3);
  tft.setSwapBytes(true);

  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(false);
  TJpgDec.setCallback(tft_output);
}

void loop()
{
  TJpgDec.drawJpg(0, 0, p1, sizeof(p1));
  delay(2000);
  TJpgDec.drawJpg(0, 0, p2, sizeof(p2));
  delay(2000);
  TJpgDec.drawJpg(0, 0, p4, sizeof(p4));
  delay(2000);
}
