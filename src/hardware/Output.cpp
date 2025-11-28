#include "Output.h"

Output output;

void Output::begin() {
  // Configurar LEDs
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);
  
  // Configurar Buzzer con PWM (canal 0)
  ledcSetup(0, BUZZER_FREQ_MED, 8); // Canal 0, frecuencia resonante, 8 bits resolución
  ledcAttachPin(PIN_BUZZER, 0);
  ledcWrite(0, 0); // Apagado inicial
  
  // Inicializar variables
  greenState = LED_OFF;
  redState = LED_OFF;
  greenLEDOn = false;
  redLEDOn = false;
  lastGreenBlink = 0;
  lastRedBlink = 0;
  
  Serial.println("Output initialized (LEDs + Buzzer TMB12A05)");
}

void Output::update() {
  updateLED(PIN_LED_GREEN, greenState, greenLEDOn, lastGreenBlink);
  updateLED(PIN_LED_RED, redState, redLEDOn, lastRedBlink);
}

void Output::updateLED(uint8_t pin, LEDState state, bool &ledOn, unsigned long &lastBlink) {
  unsigned long now = millis();
  
  switch (state) {
    case LED_OFF:
      digitalWrite(pin, LOW);
      ledOn = false;
      break;
      
    case LED_ON:
      digitalWrite(pin, HIGH);
      ledOn = true;
      break;
      
    case LED_BLINK_FAST:
      if (now - lastBlink >= LED_BLINK_FAST_MS) {
        ledOn = !ledOn;
        digitalWrite(pin, ledOn ? HIGH : LOW);
        lastBlink = now;
      }
      break;

    case LED_BLINK_SLOW:
      if (now - lastBlink >= LED_BLINK_SLOW_MS) {
        ledOn = !ledOn;
        digitalWrite(pin, ledOn ? HIGH : LOW);
        lastBlink = now;
      }
      break;
  }
}

void Output::setGreenLED(LEDState state) {
  greenState = state;
}

void Output::setRedLED(LEDState state) {
  redState = state;
}

void Output::greenOn() {
  setGreenLED(LED_ON);
}

void Output::greenOff() {
  setGreenLED(LED_OFF);
}

void Output::redOn() {
  setRedLED(LED_ON);
}

void Output::redOff() {
  setRedLED(LED_OFF);
}

void Output::allLedsOff() {
  greenOff();
  redOff();
}

// ===== BUZZER MELODÍAS =====

void Output::playBeep() {
  ledcWriteTone(0, 2300); // Frecuencia resonante TMB12A05
  delay(100);
  ledcWriteTone(0, 0);
}

void Output::playError() {
  ledcWriteTone(0, 1000); // Tono grave
  delay(300);
  ledcWriteTone(0, 0);
}

void Output::playSuccess() {
  // Tono ascendente
  ledcWriteTone(0, 2000);
  delay(100);
  ledcWriteTone(0, 2500);
  delay(100);
  ledcWriteTone(0, 3000);
  delay(150);
  ledcWriteTone(0, 0);
}

void Output::playPomodoroComplete() {
  // Melodía especial para fin de Pomodoro
  // Patrón: beep-beep-beeeeep
  
  // Beep 1
  ledcWriteTone(0, 2300);
  delay(200);
  ledcWriteTone(0, 0);
  delay(150);
  
  // Beep 2
  ledcWriteTone(0, 2300);
  delay(200);
  ledcWriteTone(0, 0);
  delay(150);
  
  // Beep 3 (largo)
  ledcWriteTone(0, 2300);
  delay(600);
  ledcWriteTone(0, 0);
  
  Serial.println("🔊 Pomodoro Complete!");
}

void Output::playBreakTime() {
  // Melodía suave para break
  // Patrón: beep-pausa-beep
  
  ledcWriteTone(0, 2000);
  delay(150);
  ledcWriteTone(0, 0);
  delay(100);
  
  ledcWriteTone(0, 2500);
  delay(200);
  ledcWriteTone(0, 0);
  
  Serial.println("🔊 Break Time!");
}

void Output::stopBuzzer() {
  ledcWriteTone(0, 0);
}