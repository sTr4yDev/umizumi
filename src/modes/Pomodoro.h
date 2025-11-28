#ifndef POMODORO_H
#define POMODORO_H

#include <Arduino.h>
#include "../hardware/Storage.h"
#include "../hardware/Output.h"
#include "../hardware/Display.h"
#include "../ui/Screens.h"

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
  
  // Estado
  PomodoroState getState();
  const char* getStateString();
  unsigned long getTimeLeft();
  uint32_t getPomodorosCompleted();
  
  // Configuración
  void enterConfigMode();
  void exitConfigMode();
  bool isInConfigMode();
  void adjustConfigValue(int potValue);
  void nextConfigField();
  void saveConfiguration();
  
private:
  PomodoroState state;
  PomodoroConfig config;
  Statistics stats;
  
  unsigned long stateStartTime;
  unsigned long sessionDuration;
  unsigned long pausedTimeRemaining;
  
  uint8_t cycleCount; // Cuenta ciclos para long break
  
  bool configMode;
  int configField; // 0=work, 1=short, 2=long, 3=cycles
  int lastConfigPotValue;
  
  void transitionTo(PomodoroState newState);
  void finishPomodoro();
  void finishBreak();
  void playStateSound();
};

extern Pomodoro pomodoro;

#endif