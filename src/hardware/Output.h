#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>
#include "../config/pins.h"
#include "../config/settings.h"

// ⭐ NO definir LEDState aquí, está en pins.h

class Output {
public:
  void begin();
  void update();
  
  // LEDs
  void setGreenLED(LEDState state);
  void setRedLED(LEDState state);
  void allLedsOff();
  
  // Buzzer
  void playTone(int freq, int duration);
  void playBeep();
  void playSuccess();
  void playWarning();
  void playError();
  
private:
  void updateLED(uint8_t pin, LEDState state, bool& ledState, unsigned long& lastBlink);
  
  bool greenLedState;
  bool redLedState;
  unsigned long greenLastBlink;
  unsigned long redLastBlink;
  LEDState greenLedMode;
  LEDState redLedMode;
};

extern Output output;

#endif