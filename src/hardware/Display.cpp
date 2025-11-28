#include "Display.h"

// ✅ Instancia GLOBAL del TFT (igual que el test que funcionó)
Adafruit_ST7735 tft = Adafruit_ST7735(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

// Instancia del wrapper Display
Display display;

void Display::begin() {
  Serial.println("Initializing display...");
  
  // ✅ Igual que el test debug que funcionó
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  tft.setTextWrap(false);
  
  Serial.println("Display initialized ✓");
}

void Display::clear() {
  tft.fillScreen(COLOR_BG);
}

void Display::setRotation(uint8_t rotation) {
  tft.setRotation(rotation);
}

void Display::drawText(const char* text, int x, int y, uint16_t color, uint8_t size) {
  tft.setTextSize(size);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(text);
}

void Display::drawCentered(const char* text, int y, uint16_t color, uint8_t size) {
  tft.setTextSize(size);
  tft.setTextColor(color);
  
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  int x = (tft.width() - w) / 2;
  
  tft.setCursor(x, y);
  tft.print(text);
}

void Display::drawRightAligned(const char* text, int y, uint16_t color, uint8_t size) {
  tft.setTextSize(size);
  tft.setTextColor(color);
  
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  int x = tft.width() - w - 4;
  
  tft.setCursor(x, y);
  tft.print(text);
}

void Display::drawRect(int x, int y, int w, int h, uint16_t color) {
  tft.drawRect(x, y, w, h, color);
}

void Display::fillRect(int x, int y, int w, int h, uint16_t color) {
  tft.fillRect(x, y, w, h, color);
}

void Display::drawLine(int x0, int y0, int x1, int y1, uint16_t color) {
  tft.drawLine(x0, y0, x1, y1, color);
}

void Display::drawCircle(int x, int y, int r, uint16_t color) {
  tft.drawCircle(x, y, r, color);
}

void Display::fillCircle(int x, int y, int r, uint16_t color) {
  tft.fillCircle(x, y, r, color);
}

void Display::fillScreen(uint16_t color) {
  tft.fillScreen(color);
}

int Display::getWidth() {
  return tft.width();
}

int Display::getHeight() {
  return tft.height();
}

Adafruit_ST7735* Display::getTFT() {
  return &tft;
}