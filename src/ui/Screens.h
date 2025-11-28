#ifndef SCREENS_H
#define SCREENS_H

#include <Arduino.h>
#include "../hardware/Display.h"
#include "../hardware/Storage.h"
#include "../config/colors.h"

struct PomodoroConfig;

class Screens {
public:
  // ⭐ SOLO UNA declaración de drawStats
  static void drawStats(Statistics& stats);
  
  // Pomodoro
  static void drawPomodoroConfig(PomodoroConfig &config, int selectedField);
  static void drawPomodoroRunning(const char* state, unsigned long timeLeft, uint32_t pomodorosCount);
  
  // Gym
  static void drawGymRunning(unsigned long elapsed, uint32_t seriesCount);
  
  // Sistema
  static void drawSettings();
  static void drawInfo();
  
  // Utilidades
  static void drawProgressBar(int x, int y, int width, int height, int percent, uint16_t color);
  static void drawTimeString(char* buffer, unsigned long seconds);
};

#endif