#ifndef SCREENS_H
#define SCREENS_H

#include "../hardware/Display.h"
#include "../hardware/Storage.h"
#include "../config/colors.h"

class Screens {
public:
  static void drawPomodoroConfig(PomodoroConfig &config, int selectedField);
  static void drawPomodoroRunning(const char* state, unsigned long timeLeft, uint32_t pomodorosCount);
  static void drawGymRunning(unsigned long elapsed, uint32_t seriesCount);
  static void drawStats(const Statistics &stats);
  static void drawSettings();
  static void drawInfo();
  
  // Utilidades
  static void drawProgressBar(int x, int y, int width, int height, int percent, uint16_t color);
  static void drawTimeString(char* buffer, unsigned long seconds);
};

#endif