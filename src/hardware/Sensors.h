#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "config/pins.h"
#include "config/settings.h"

class Sensors {
public:
  void begin();
  void update();
  
  // CLAP detection
  bool clapDetected();
  void resetClap();
  
  // ⭐ NUEVO: Obtener valores analógicos
  int getAnalogValue();
  float getVoltage();
  float getApproxDB();
  int getBaselineValue();
  
private:
  // Digital detection
  bool soundState;
  bool soundLast;
  unsigned long soundStartTime;
  unsigned long lastClapTime;
  bool clapFlag;
  
  // ⭐ NUEVO: Mediciones analógicas
  int analogValue;
  int baselineValue;      // Valor de ruido base
  int peakValue;          // Pico más alto detectado
  float smoothedValue;    // Valor suavizado
};

extern Sensors sensors;

#endif  // SENSORS_H