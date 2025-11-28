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
  int rawValue = digitalRead(PIN_SOUND);
  soundState = (rawValue == LOW); // Prueba con LOW primero
  
  // ✅ DEBUG: Imprimir cuando cambia
  static int lastRaw = -1;
  if (rawValue != lastRaw) {
    Serial.print("🔊 Sound sensor: ");
    Serial.println(rawValue ? "HIGH" : "LOW");
    lastRaw = rawValue;
  }
  
  // Detectar inicio de sonido
  if (soundState && !soundLast) {
    soundStartTime = millis();
    Serial.println("🎤 Sound START");  // ✅ DEBUG
  }
  
  // Detectar fin de sonido
  if (!soundState && soundLast) {
    unsigned long duration = millis() - soundStartTime;
    unsigned long timeSinceLastClap = millis() - lastClapTime;
    
    Serial.print("🎤 Sound END - Duration: ");
    Serial.print(duration);
    Serial.println("ms");  // ✅ DEBUG
    
    // Verificar si es un CLAP válido
    if (duration >= CLAP_DURATION_MIN && 
        duration <= CLAP_DURATION_MAX && 
        timeSinceLastClap >= CLAP_COOLDOWN_MS) {
      
      clapFlag = true;
      lastClapTime = millis();
      
      Serial.print("👏 CLAP VÁLIDO! Duración: ");
      Serial.print(duration);
      Serial.println("ms");
    } else {
      Serial.print("❌ Ruido ignorado - ");
      if (duration < CLAP_DURATION_MIN) Serial.print("muy corto");
      else if (duration > CLAP_DURATION_MAX) Serial.print("muy largo");
      else if (timeSinceLastClap < CLAP_COOLDOWN_MS) Serial.print("muy rápido");
      Serial.println();
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