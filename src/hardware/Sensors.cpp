#include "Sensors.h"

Sensors sensors;

void Sensors::begin() {
  pinMode(PIN_SOUND, INPUT);
  
  soundState = false;
  soundLast = false;
  soundStartTime = 0;
  lastClapTime = 0;
  clapFlag = false;
  
  Serial.println("Sensors initialized (CLAP detector)");
}

void Sensors::update() {
  soundState = digitalRead(PIN_SOUND) == LOW; // Activo LOW
  
  // Detectar inicio de sonido
  if (soundState && !soundLast) {
    soundStartTime = millis();
  }
  
  // Detectar fin de sonido
  if (!soundState && soundLast) {
    unsigned long duration = millis() - soundStartTime;
    unsigned long timeSinceLastClap = millis() - lastClapTime;
    
    // Verificar si es un CLAP válido
    if (duration >= CLAP_DURATION_MIN && 
        duration <= CLAP_DURATION_MAX && 
        timeSinceLastClap >= CLAP_COOLDOWN_MS) {
      
      clapFlag = true;
      lastClapTime = millis();
      
      Serial.print("👏 CLAP detectado! Duración: ");
      Serial.print(duration);
      Serial.println("ms");
    }
  }
  
  soundLast = soundState;
}

bool Sensors::clapDetected() {
  return clapFlag;
}

void Sensors::resetClap() {
  clapFlag = false;
}