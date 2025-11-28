#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "../config/pins.h"
#include "../config/settings.h"

class Sensors {
public:
  void begin();
  void update();
  
  // Detector de CLAP
  bool clapDetected();
  void resetClap();
  
private:
  // Sound sensor
  bool soundState;
  bool soundLast;
  unsigned long soundStartTime;
  unsigned long lastClapTime;
  bool clapFlag;
};

extern Sensors sensors;

#endif