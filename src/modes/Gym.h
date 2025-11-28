#ifndef GYM_H
#define GYM_H

#include <Arduino.h>
#include "../hardware/Storage.h"
#include "../hardware/Output.h"
#include "../hardware/Sensors.h"
#include "../hardware/Display.h"
#include "../ui/Screens.h"

enum GymState {
  GYM_IDLE,
  GYM_RUNNING,
  GYM_PAUSED
};

class Gym {
public:
  void begin();
  void update();
  void draw();
  
  // Control con CLAP
  void handleClap();
  
  // Control manual
  void start();
  void pause();
  void resume();
  void stop();
  void reset();
  
  // Estado
  GymState getState();
  unsigned long getElapsedTime();
  uint32_t getSeriesCount();
  
private:
  GymState state;
  GymConfig config;
  Statistics stats;
  
  unsigned long sessionStartTime;
  unsigned long pausedElapsed;
  unsigned long lastSerieTime;
  
  void addSerie();
};

extern Gym gym;

#endif