#include "Input.h"

Input input;

void Input::begin() {
  // Configurar botones con pull-up interno
  pinMode(PIN_BTN_SELECT, INPUT_PULLUP);
  pinMode(PIN_BTN_BACK, INPUT_PULLUP);
  
  // Inicializar potenciómetro
  pinMode(PIN_POT, INPUT);
  
  // Inicializar variables
  potRaw = 0;
  potSmoothed = 0;
  potLast = 0;
  potIndex = 0;
  for (int i = 0; i < POT_SAMPLES; i++) {
    potSamples[i] = 0;
  }
  
  btnSelectState = HIGH;
  btnSelectLast = HIGH;
  btnBackState = HIGH;
  btnBackLast = HIGH;
  btnSelectPressTime = 0;
  btnBackPressTime = 0;
  lastDebounceTime = 0;
  
  Serial.println("Input initialized");
}

void Input::update() {
  // Leer potenciómetro con suavizado
  potRaw = analogRead(PIN_POT);
  
  // Promediar muestras
  potSamples[potIndex] = potRaw;
  potIndex = (potIndex + 1) % POT_SAMPLES;
  
  int sum = 0;
  for (int i = 0; i < POT_SAMPLES; i++) {
    sum += potSamples[i];
  }
  potSmoothed = sum / POT_SAMPLES;
  
  // Leer botones
  btnSelectState = digitalRead(PIN_BTN_SELECT);
  btnBackState = digitalRead(PIN_BTN_BACK);
}

int Input::getPotValue() {
  return potSmoothed;
}

int Input::getPotPosition() {
  // Convertir 0-4095 a 0-100
  return map(potSmoothed, 0, 4095, 0, 100);
}

bool Input::potChanged() {
  int diff = abs(potSmoothed - potLast);
  if (diff > POT_THRESHOLD) {
    potLast = potSmoothed;
    return true;
  }
  return false;
}

ButtonEvent Input::getButtonEvent() {
  unsigned long now = millis();
  
  // Debounce
  if (now - lastDebounceTime < 50) {
    return BTN_NONE;
  }
  
  // SELECT
  if (btnSelectState != btnSelectLast) {
    lastDebounceTime = now;
    btnSelectLast = btnSelectState;
    
    if (btnSelectState == LOW) {
      // Presionado
      btnSelectPressTime = now;
      return BTN_SELECT_PRESSED;
    } else {
      // Liberado
      unsigned long pressDuration = now - btnSelectPressTime;
      if (pressDuration > 1000) {
        return BTN_SELECT_LONG_PRESS;
      } else {
        return BTN_SELECT_RELEASED;
      }
    }
  }
  
  // BACK
  if (btnBackState != btnBackLast) {
    lastDebounceTime = now;
    btnBackLast = btnBackState;
    
    if (btnBackState == LOW) {
      btnBackPressTime = now;
      return BTN_BACK_PRESSED;
    } else {
      unsigned long pressDuration = now - btnBackPressTime;
      if (pressDuration > 1000) {
        return BTN_BACK_LONG_PRESS;
      } else {
        return BTN_BACK_RELEASED;
      }
    }
  }
  
  return BTN_NONE;
}

bool Input::isSelectPressed() {
  return btnSelectState == LOW;
}

bool Input::isBackPressed() {
  return btnBackState == LOW;
}