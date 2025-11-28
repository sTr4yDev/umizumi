#include "Pomodoro.h"

Pomodoro pomodoro;

void Pomodoro::begin() {
  state = POM_IDLE;
  stateStartTime = 0;
  sessionDuration = 0;
  pausedTimeRemaining = 0;
  cycleCount = 0;
  
  configMode = false;
  configField = 0;
  lastConfigPotValue = 0;
  
  // Cargar configuración
  storage.loadPomodoroConfig(config);
  storage.loadStats(stats);
  
  Serial.println("Pomodoro mode initialized");
  Serial.printf("Config: Work=%dmin, Short=%dmin, Long=%dmin\n",
                config.workTime / 60, config.shortBreak / 60, config.longBreak / 60);
}

void Pomodoro::update() {
  if (configMode) {
    // En modo configuración, no avanza el timer
    return;
  }
  
  unsigned long now = millis();
  
  switch (state) {
    case POM_RUNNING:
    case POM_SHORT_BREAK:
    case POM_LONG_BREAK: {
      unsigned long elapsed = (now - stateStartTime) / 1000;
      
      if (elapsed >= sessionDuration) {
        // Tiempo completado
        if (state == POM_RUNNING) {
          finishPomodoro();
        } else {
          finishBreak();
        }
      }
      break;
    }
    
    default:
      break;
  }
}

void Pomodoro::draw() {
  if (configMode) {
    Screens::drawPomodoroConfig(config, configField);
  } else {
    unsigned long timeLeft = getTimeLeft();
    Screens::drawPomodoroRunning(getStateString(), timeLeft, stats.pomodorosCompleted);
  }
}

void Pomodoro::start() {
  if (state == POM_IDLE || state == POM_PAUSED) {
    if (state == POM_IDLE) {
      // Iniciar nuevo pomodoro
      sessionDuration = config.workTime;
      stateStartTime = millis();
    } else {
      // Resumir desde pausa
      sessionDuration = pausedTimeRemaining;
      stateStartTime = millis();
    }
    
    transitionTo(POM_RUNNING);
    output.setGreenLED(LED_ON);
    output.playSuccess();
    
    Serial.println("▶ Pomodoro started");
  }
}

void Pomodoro::pause() {
  if (state == POM_RUNNING) {
    unsigned long elapsed = (millis() - stateStartTime) / 1000;
    pausedTimeRemaining = (elapsed < sessionDuration) ? (sessionDuration - elapsed) : 0;
    
    transitionTo(POM_PAUSED);
    output.setGreenLED(LED_OFF);
    output.setRedLED(LED_BLINK_SLOW);
    output.playBeep();
    
    Serial.printf("⏸ Pomodoro paused (%lu sec remaining)\n", pausedTimeRemaining);
  } else if (state == POM_SHORT_BREAK || state == POM_LONG_BREAK) {
    unsigned long elapsed = (millis() - stateStartTime) / 1000;
    pausedTimeRemaining = (elapsed < sessionDuration) ? (sessionDuration - elapsed) : 0;
    
    transitionTo(POM_BREAK_PAUSED);
    output.playBeep();
    
    Serial.println("⏸ Break paused");
  }
}

void Pomodoro::resume() {
  if (state == POM_PAUSED) {
    sessionDuration = pausedTimeRemaining;
    stateStartTime = millis();
    transitionTo(POM_RUNNING);
    output.setGreenLED(LED_ON);
    output.setRedLED(LED_OFF);
    output.playBeep();
    
    Serial.println("▶ Pomodoro resumed");
  } else if (state == POM_BREAK_PAUSED) {
    sessionDuration = pausedTimeRemaining;
    stateStartTime = millis();
    
    // Volver al break correspondiente
    if (cycleCount % config.cyclesUntilLong == 0) {
      transitionTo(POM_LONG_BREAK);
    } else {
      transitionTo(POM_SHORT_BREAK);
    }
    
    output.playBeep();
    Serial.println("▶ Break resumed");
  }
}

void Pomodoro::stop() {
  transitionTo(POM_IDLE);
  output.allLedsOff();
  output.playBeep();
  
  Serial.println("⏹ Pomodoro stopped");
}

void Pomodoro::skipBreak() {
  if (state == POM_SHORT_BREAK || state == POM_LONG_BREAK || state == POM_BREAK_PAUSED) {
    finishBreak();
    output.playBeep();
    
    Serial.println("⏭ Break skipped");
  }
}

void Pomodoro::finishPomodoro() {
  // Incrementar contador
  stats.pomodorosCompleted++;
  cycleCount++;
  
  // Agregar tiempo trabajado
  storage.addWorkTime(config.workTime);
  
  // Guardar stats
  storage.saveStats(stats);
  
  Serial.printf("✅ Pomodoro #%lu completed!\n", stats.pomodorosCompleted);
  
  // Determinar tipo de break
  if (cycleCount % config.cyclesUntilLong == 0) {
    // Long break cada N pomodoros
    sessionDuration = config.longBreak;
    transitionTo(POM_LONG_BREAK);
    Serial.println("🌴 Starting LONG break");
  } else {
    // Short break
    sessionDuration = config.shortBreak;
    transitionTo(POM_SHORT_BREAK);
    Serial.println("☕ Starting SHORT break");
  }
  
  stateStartTime = millis();
  
  // Feedback
  output.setGreenLED(LED_OFF);
  output.setRedLED(LED_ON);
  output.playPomodoroComplete();
}

void Pomodoro::finishBreak() {
  transitionTo(POM_IDLE);
  output.allLedsOff();
  output.playBreakTime();
  
  Serial.println("✅ Break finished - Ready for next pomodoro");
}

void Pomodoro::transitionTo(PomodoroState newState) {
  state = newState;
  playStateSound();
}

void Pomodoro::playStateSound() {
  switch (state) {
    case POM_RUNNING:
      // Sonido ya se hace en start()
      break;
      
    case POM_SHORT_BREAK:
    case POM_LONG_BREAK:
      output.playBreakTime();
      break;
      
    default:
      break;
  }
}

PomodoroState Pomodoro::getState() {
  return state;
}

const char* Pomodoro::getStateString() {
  switch (state) {
    case POM_IDLE: return "Idle";
    case POM_RUNNING: return "Work";
    case POM_PAUSED: return "Paused";
    case POM_SHORT_BREAK: return "Break";
    case POM_LONG_BREAK: return "Long Break";
    case POM_BREAK_PAUSED: return "Break Paused";
    default: return "Unknown";
  }
}

unsigned long Pomodoro::getTimeLeft() {
  if (state == POM_IDLE) {
    return config.workTime;
  } else if (state == POM_PAUSED || state == POM_BREAK_PAUSED) {
    return pausedTimeRemaining;
  } else {
    unsigned long elapsed = (millis() - stateStartTime) / 1000;
    if (elapsed >= sessionDuration) {
      return 0;
    }
    return sessionDuration - elapsed;
  }
}

uint32_t Pomodoro::getPomodorosCompleted() {
  return stats.pomodorosCompleted;
}

// ===== CONFIGURACIÓN =====

void Pomodoro::enterConfigMode() {
  configMode = true;
  configField = 0;
  lastConfigPotValue = 0;
  
  Serial.println("⚙️ Entering config mode");
}

void Pomodoro::exitConfigMode() {
  configMode = false;
  
  Serial.println("⚙️ Exiting config mode");
}

bool Pomodoro::isInConfigMode() {
  return configMode;
}

void Pomodoro::adjustConfigValue(int potValue) {
  if (!configMode) return;
  
  // Detectar cambio significativo
  if (abs(potValue - lastConfigPotValue) < 5) return;
  lastConfigPotValue = potValue;
  
  switch (configField) {
    case 0: // Work time (5-60 min)
      config.workTime = map(potValue, 0, 100, 5, 60) * 60;
      break;
      
    case 1: // Short break (1-15 min)
      config.shortBreak = map(potValue, 0, 100, 1, 15) * 60;
      break;
      
    case 2: // Long break (10-30 min)
      config.longBreak = map(potValue, 0, 100, 10, 30) * 60;
      break;
      
    case 3: // Cycles (2-8)
      config.cyclesUntilLong = map(potValue, 0, 100, 2, 8);
      break;
  }
  
  output.playBeep();
}

void Pomodoro::nextConfigField() {
  configField = (configField + 1) % 4;
  output.playBeep();
  
  Serial.printf("Config field: %d\n", configField);
}

void Pomodoro::saveConfiguration() {
  storage.savePomodoroConfig(config);
  output.playSuccess();
  
  Serial.println("✅ Configuration saved!");
}