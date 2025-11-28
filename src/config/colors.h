#ifndef COLORS_H
#define COLORS_H

#include <Adafruit_ST7735.h>

// ===== COLORES TEMA OSCURO =====
#define COLOR_BG          ST77XX_BLACK
#define COLOR_TEXT        ST77XX_WHITE
#define COLOR_PRIMARY     0x07FF        // Cyan
#define COLOR_SECONDARY   0xFD20        // Orange
#define COLOR_SUCCESS     ST77XX_GREEN
#define COLOR_WARNING     ST77XX_YELLOW
#define COLOR_DANGER      ST77XX_RED
#define COLOR_INFO        0x051D        // Azul oscuro

// ===== COLORES ESTADOS =====
#define COLOR_IDLE        ST77XX_WHITE
#define COLOR_RUNNING     ST77XX_GREEN
#define COLOR_PAUSED      ST77XX_ORANGE
#define COLOR_BREAK       0x07FF        // Cyan

// ===== COLORES MENÚ =====
#define COLOR_MENU_BG     ST77XX_BLACK
#define COLOR_MENU_TEXT   ST77XX_WHITE
#define COLOR_MENU_SELECT 0x07FF        // Cyan
#define COLOR_MENU_CURSOR 0xFD20        // Orange

#endif