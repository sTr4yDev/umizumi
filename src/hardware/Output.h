#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>
#include "../config/pins.h"
#include "../config/settings.h"

enum LEDState {
  LED_OFF,
  LED_ON,
  LED_BLINK_FAST,
  LED_BLINK_SLOW
};

class Output {
public:
  void begin();
  void update();
  
  // LEDs
  void setGreenLED(LEDState state);
  void setRedLED(LEDState state);
  void greenOn();
  void greenOff();
  void redOn();
  void redOff();
  void allLedsOff();
  
  // Buzzer
  void playBeep();
  void playError();
  void playSuccess();
  void playPomodoroComplete();
  void playBreakTime();
  void stopBuzzer();
  
private:
  // LEDs
  LEDState greenState;
  LEDState redState;
  bool greenLEDOn;
  bool redLEDOn;
  unsigned long lastGreenBlink;
  unsigned long lastRedBlink;
  
  void updateLED(uint8_t pin, LEDState state, bool &ledOn, unsigned long &lastBlink);
};

extern Output output;

#endif