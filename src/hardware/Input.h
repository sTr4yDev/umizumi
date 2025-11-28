#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "../config/pins.h"
#include "../config/settings.h"

enum ButtonEvent {
  BTN_NONE,
  BTN_SELECT_PRESSED,
  BTN_SELECT_RELEASED,
  BTN_SELECT_LONG_PRESS,
  BTN_BACK_PRESSED,
  BTN_BACK_RELEASED,
  BTN_BACK_LONG_PRESS
};

class Input {
public:
  void begin();
  void update();
  
  // Potenciómetro
  int getPotValue();          // 0-4095 raw
  int getPotPosition();       // 0-100 normalizado
  bool potChanged();          // Detecta cambio significativo
  
  // Botones
  ButtonEvent getButtonEvent();
  bool isSelectPressed();
  bool isBackPressed();
  
private:
  // Potenciómetro
  int potRaw;
  int potSmoothed;
  int potLast;
  int potSamples[POT_SAMPLES];
  int potIndex;
  
  // Botones
  bool btnSelectState;
  bool btnSelectLast;
  bool btnBackState;
  bool btnBackLast;
  unsigned long btnSelectPressTime;
  unsigned long btnBackPressTime;
  unsigned long lastDebounceTime;
};

extern Input input;

#endif