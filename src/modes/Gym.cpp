#include "Gym.h"

Gym gym;

void Gym::begin() {
  state = GYM_IDLE;
  sessionStartTime = 0;
  pausedElapsed = 0;
  lastSerieTime = 0;
  
  // Cargar configuración
  storage.loadGymConfig(config);
  storage.loadStats(stats);
  
  Serial.println("Gym mode initialized");
}

void Gym::update() {
  // Detector de CLAP
  if (sensors.clapDetected()) {
    handleClap();
    sensors.resetClap();
  }
}

void Gym::draw() {
  unsigned long elapsed = getElapsedTime();
  Screens::drawGymRunning(elapsed, stats.gymSeriesCompleted);
}

void Gym::handleClap() {
  Serial.println("👏 CLAP detected in Gym mode");
  
  if (state == GYM_IDLE || state == GYM_PAUSED) {
    start();
  } else if (state == GYM_RUNNING) {
    pause();
  }
}

void Gym::start() {
  if (state == GYM_IDLE) {
    sessionStartTime = millis();
    pausedElapsed = 0;
    state = GYM_RUNNING;
    
    output.setGreenLED(LED_ON);
    output.playSuccess();
    
    Serial.println("▶ Gym session started");
  } else if (state == GYM_PAUSED) {
    resume();
  }
}

void Gym::pause() {
  if (state == GYM_RUNNING) {
    pausedElapsed = (millis() - sessionStartTime) / 1000;
    state = GYM_PAUSED;
    
    output.setGreenLED(LED_OFF);
    output.setRedLED(LED_BLINK_SLOW);
    output.playBeep();
    
    Serial.println("⏸ Gym session paused");
  }
}

void Gym::resume() {
  if (state == GYM_PAUSED) {
    sessionStartTime = millis() - (pausedElapsed * 1000);
    state = GYM_RUNNING;
    
    output.setGreenLED(LED_ON);
    output.setRedLED(LED_OFF);
    output.playBeep();
    
    Serial.println("▶ Gym session resumed");
  }
}

void Gym::stop() {
  state = GYM_IDLE;
  sessionStartTime = 0;
  pausedElapsed = 0;
  
  output.allLedsOff();
  output.playBeep();
  
  Serial.println("⏹ Gym session stopped");
}

void Gym::reset() {
  stop();
  
  // Resetear contador de series de la sesión actual
  // (no afecta el total guardado)
  
  Serial.println("🔄 Gym session reset");
}

void Gym::addSerie() {
  unsigned long now = millis();
  
  // Evitar doble conteo (cooldown de 2 segundos)
  if (now - lastSerieTime < 2000) {
    return;
  }
  
  lastSerieTime = now;
  
  stats.gymSeriesCompleted++;
  storage.saveStats(stats);
  
  // Feedback visual y sonoro
  output.setGreenLED(LED_BLINK_FAST);
  output.playSuccess();
  
  delay(100);
  output.setGreenLED(state == GYM_RUNNING ? LED_ON : LED_OFF);
  
  Serial.printf("💪 Serie #%lu completed!\n", stats.gymSeriesCompleted);
}

GymState Gym::getState() {
  return state;
}

unsigned long Gym::getElapsedTime() {
  if (state == GYM_IDLE) {
    return 0;
  } else if (state == GYM_PAUSED) {
    return pausedElapsed;
  } else {
    return (millis() - sessionStartTime) / 1000;
  }
}

uint32_t Gym::getSeriesCount() {
  return stats.gymSeriesCompleted;
}