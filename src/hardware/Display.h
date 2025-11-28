#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "../config/pins.h"
#include "../config/colors.h"

class Display {
public:
  Display();
  void begin();
  void clear();
  void setRotation(uint8_t rotation);
  
  // Texto
  void drawText(const char* text, int x, int y, uint16_t color, uint8_t size = 1);
  void drawCentered(const char* text, int y, uint16_t color, uint8_t size = 2);
  void drawRightAligned(const char* text, int y, uint16_t color, uint8_t size = 1);
  
  // Formas
  void drawRect(int x, int y, int w, int h, uint16_t color);
  void fillRect(int x, int y, int w, int h, uint16_t color);
  void drawLine(int x0, int y0, int x1, int y1, uint16_t color);
  void drawCircle(int x, int y, int r, uint16_t color);
  void fillCircle(int x, int y, int r, uint16_t color);
  
  // Utilidades
  void fillScreen(uint16_t color);
  int getWidth();
  int getHeight();
  
  // Acceso directo al TFT (para casos especiales)
  Adafruit_ST7735* getTFT();
  
private:
  Adafruit_ST7735 tft;
};

extern Display display;

#endif