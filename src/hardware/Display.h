#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "../config/pins.h"
#include "../config/colors.h"

// ✅ TFT global (como el test que funcionó)
extern Adafruit_ST7735 tft;

class Display {
public:
  static void begin();
  static void clear();
  static void setRotation(uint8_t rotation);
  
  // Texto
  static void drawText(const char* text, int x, int y, uint16_t color, uint8_t size = 1);
  static void drawCentered(const char* text, int y, uint16_t color, uint8_t size = 2);
  static void drawRightAligned(const char* text, int y, uint16_t color, uint8_t size = 1);
  
  // Formas
  static void drawRect(int x, int y, int w, int h, uint16_t color);
  static void fillRect(int x, int y, int w, int h, uint16_t color);
  static void drawLine(int x0, int y0, int x1, int y1, uint16_t color);
  static void drawCircle(int x, int y, int r, uint16_t color);
  static void fillCircle(int x, int y, int r, uint16_t color);
  
  // Utilidades
  static void fillScreen(uint16_t color);
  static int getWidth();
  static int getHeight();
  static Adafruit_ST7735* getTFT();
};

extern Display display;

#endif