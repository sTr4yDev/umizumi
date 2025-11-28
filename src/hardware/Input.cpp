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
  ButtonEvent event = BTN_NONE;
  
  // Debounce
  if (now - lastDebounceTime < 50) {
    return BTN_NONE;
  }
  
  // SELECT - ✅ CORREGIDO: Mejor manejo de estados
  if (btnSelectState != btnSelectLast) {
    lastDebounceTime = now;
    
    if (btnSelectState == LOW) {
      // Presionado
      btnSelectPressTime = now;
      Serial.println("🔘 SELECT pressed");
      event = BTN_SELECT_PRESSED;
    } else {
      // Liberado
      unsigned long pressDuration = now - btnSelectPressTime;
      Serial.print("🔘 SELECT released (");
      Serial.print(pressDuration);
      Serial.println("ms)");
      
      if (pressDuration > 1000) {
        Serial.println("🔘 SELECT LONG PRESS DETECTED");
        event = BTN_SELECT_LONG_PRESS;
      } else {
        Serial.println("🔘 SELECT short press");
        event = BTN_SELECT_RELEASED;
      }
    }
    // ✅ IMPORTANTE: Actualizar el estado anterior DESPUÉS de procesar
    btnSelectLast = btnSelectState;
  }
  
  // Si ya hay evento de SELECT, no procesar BACK
  if (event != BTN_NONE) {
    return event;
  }
  
  // BACK
  if (btnBackState != btnBackLast) {
    lastDebounceTime = now;
    
    if (btnBackState == LOW) {
      // Presionado
      btnBackPressTime = now;
      Serial.println("🔘 BACK pressed");
      event = BTN_BACK_PRESSED;
    } else {
      // Liberado
      unsigned long pressDuration = now - btnBackPressTime;
      Serial.print("🔘 BACK released (");
      Serial.print(pressDuration);
      Serial.println("ms)");
      
      if (pressDuration > 1000) {
        Serial.println("🔘 BACK LONG PRESS DETECTED");
        event = BTN_BACK_LONG_PRESS;
      } else {
        Serial.println("🔘 BACK short press");
        event = BTN_BACK_RELEASED;
      }
    }
    // ✅ Actualizar el estado anterior DESPUÉS de procesar
    btnBackLast = btnBackState;
  }
  
  return event;
}

bool Input::isSelectPressed() {
  return btnSelectState == LOW;
}

bool Input::isBackPressed() {
  return btnBackState == LOW;
}