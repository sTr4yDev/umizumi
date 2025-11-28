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
  
  // ⭐ PROTECCIÓN: Limitar rango válido
  if (potRaw < 0) potRaw = 0;
  if (potRaw > 4095) potRaw = 4095;
  
  // Promediar muestras
  potSamples[potIndex] = potRaw;
  potIndex = (potIndex + 1) % POT_SAMPLES;
  
  int sum = 0;
  for (int i = 0; i < POT_SAMPLES; i++) {
    sum += potSamples[i];
  }
  potSmoothed = sum / POT_SAMPLES;
  
  // ⭐ FILTRO ANTI-RUIDO: Ignorar valores extremos anormales
  if (potSmoothed < 10) potSmoothed = 0;
  if (potSmoothed > 4085) potSmoothed = 4095;
  
  // Leer botones (invertido porque usamos INPUT_PULLUP)
  btnSelectState = digitalRead(PIN_BTN_SELECT);
  btnBackState = digitalRead(PIN_BTN_BACK);
}

int Input::getPotValue() {
  return potSmoothed;
}

int Input::getPotPosition() {
  // Convertir 0-4095 a 0-100
  int pos = map(potSmoothed, 0, 4095, 0, 100);
  
  // ⭐ CLAMP: Asegurar que esté en rango
  if (pos < 0) pos = 0;
  if (pos > 100) pos = 100;
  
  return pos;
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