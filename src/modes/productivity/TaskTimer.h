#ifndef TASK_TIMER_H
#define TASK_TIMER_H

#include <Arduino.h>
#include "../../hardware/Display.h"
#include "../../hardware/Input.h"
#include "../../hardware/Output.h"
#include "../../hardware/Storage.h"
#include "../../config/colors.h"

enum TaskTimerState {
  TASK_IDLE,
  TASK_RUNNING,
  TASK_PAUSED,
  TASK_COMPLETED
};

class TaskTimer {
public:
  void begin();
  void update();
  void draw();
  
  // Control
  void start();
  void pause();
  void resume();
  void stop();
  void reset();
  
  // Estado
  TaskTimerState getState();
  unsigned long getElapsedTime();
  
private:
  TaskTimerState state;
  unsigned long startTime;
  unsigned long pausedTime;
  unsigned long totalPausedTime;
  unsigned long elapsedTime;
  
  void updateElapsedTime();
  void saveTask();
};

extern TaskTimer taskTimer;

#endif