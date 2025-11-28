#ifndef POMODORO_H
#define POMODORO_H

#include <Arduino.h>
#include "../hardware/Output.h"
#include "../hardware/Display.h"
#include "../hardware/Storage.h"
#include "../config/colors.h"

// ===== ESTADOS DEL POMODORO =====
enum PomodoroState {
  POM_IDLE,
  POM_RUNNING,
  POM_PAUSED,
  POM_SHORT_BREAK,
  POM_LONG_BREAK,
  POM_BREAK_PAUSED
};

class Pomodoro {
public:
  void begin();
  void update();
  void draw();
  
  // Control
  void start();
  void pause();
  void resume();
  void stop();
  void skipBreak();
  
  // Configuración
  void enterConfigMode();
  void exitConfigMode();
  void nextConfigField();
  void adjustConfigValue(int potValue);
  void saveConfiguration();
  
  // Estado
  PomodoroState getState();
  unsigned long getTimeLeft();
  const char* getStateString();
  
private:
  PomodoroState state;
  PomodoroConfig config;
  
  unsigned long startTime;
  unsigned long pausedTime;
  unsigned long totalPausedTime;
  unsigned long timeRemaining;
  
  int pomodorosCompleted;
  int currentCycle;
  
  bool inConfigMode;
  int selectedField;
  
  void transitionTo(PomodoroState newState);
  void finishPomodoro();
  void finishBreak();
  void playStateSound();
};

extern Pomodoro pomodoro;

#endif