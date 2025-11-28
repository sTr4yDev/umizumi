#include "TaskTimer.h"

TaskTimer taskTimer;

void TaskTimer::begin() {
  state = TASK_IDLE;
  startTime = 0;
  pausedTime = 0;
  totalPausedTime = 0;
  elapsedTime = 0;
  
  Serial.println("TaskTimer initialized");
}

void TaskTimer::update() {
  if (state == TASK_RUNNING) {
    updateElapsedTime();
  }
}

void TaskTimer::draw() {
  display.clear();
  
  // Header
  display.fillRect(0, 0, display.getWidth(), 22, COLOR_SUCCESS);
  display.drawCentered("TASK TIMER", 7, ST77XX_WHITE, 1);
  
  // Estado
  const char* stateText = "";
  uint16_t stateColor = COLOR_TEXT;
  
  switch (state) {
    case TASK_IDLE:
      stateText = "LISTO";
      stateColor = COLOR_INFO;
      break;
    case TASK_RUNNING:
      stateText = "CORRIENDO";
      stateColor = COLOR_SUCCESS;
      break;
    case TASK_PAUSED:
      stateText = "PAUSADO";
      stateColor = COLOR_WARNING;
      break;
    case TASK_COMPLETED:
      stateText = "COMPLETADO";
      stateColor = COLOR_PRIMARY;
      break;
  }
  
  display.drawCentered(stateText, 32, stateColor, 1);
  
  // Tiempo transcurrido
  unsigned long seconds = elapsedTime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  
  seconds = seconds % 60;
  minutes = minutes % 60;
  
  char timeStr[16];
  snprintf(timeStr, sizeof(timeStr), "%02lu:%02lu:%02lu", hours, minutes, seconds);
  
  display.drawCentered(timeStr, 60, COLOR_TEXT, 3);
  
  // Instrucciones
  display.setTextSize(1);
  
  if (state == TASK_IDLE) {
    display.drawCentered("SELECT: Iniciar", 110, COLOR_INFO, 1);
  } else if (state == TASK_RUNNING) {
    display.drawCentered("SELECT: Pausar", 110, COLOR_INFO, 1);
    display.drawCentered("LONG: Completar", 122, COLOR_INFO, 1);
  } else if (state == TASK_PAUSED) {
    display.drawCentered("SELECT: Reanudar", 110, COLOR_INFO, 1);
    display.drawCentered("LONG: Completar", 122, COLOR_INFO, 1);
  } else if (state == TASK_COMPLETED) {
    display.drawCentered("SELECT: Nueva Tarea", 110, COLOR_INFO, 1);
  }
  
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

void TaskTimer::start() {
  if (state == TASK_IDLE) {
    state = TASK_RUNNING;
    startTime = millis();
    totalPausedTime = 0;
    elapsedTime = 0;
    
    output.playBeep();
    output.setGreenLED(LED_ON);
    
    Serial.println("Task started");
  }
}

void TaskTimer::pause() {
  if (state == TASK_RUNNING) {
    state = TASK_PAUSED;
    pausedTime = millis();
    
    output.playBeep();
    output.setGreenLED(LED_OFF);
    output.setRedLED(LED_BLINK_SLOW);
    
    Serial.println("Task paused");
  }
}

void TaskTimer::resume() {
  if (state == TASK_PAUSED) {
    state = TASK_RUNNING;
    totalPausedTime += (millis() - pausedTime);
    
    output.playBeep();
    output.setRedLED(LED_OFF);
    output.setGreenLED(LED_ON);
    
    Serial.println("Task resumed");
  }
}

void TaskTimer::stop() {
  if (state == TASK_RUNNING || state == TASK_PAUSED) {
    updateElapsedTime();
    state = TASK_COMPLETED;
    
    saveTask();
    
    output.allLedsOff();
    output.playSuccess();
    
    Serial.print("Task completed! Duration: ");
    Serial.print(elapsedTime / 1000);
    Serial.println(" seconds");
  }
}

void TaskTimer::reset() {
  state = TASK_IDLE;
  startTime = 0;
  pausedTime = 0;
  totalPausedTime = 0;
  elapsedTime = 0;
  
  output.allLedsOff();
  
  Serial.println("Task reset");
}

TaskTimerState TaskTimer::getState() {
  return state;
}

unsigned long TaskTimer::getElapsedTime() {
  return elapsedTime;
}

void TaskTimer::updateElapsedTime() {
  if (state == TASK_RUNNING) {
    elapsedTime = millis() - startTime - totalPausedTime;
  }
}

void TaskTimer::saveTask() {
  // Guardar en estadísticas
  Statistics stats;
  storage.loadStats(stats);
  
  stats.tasksCompleted++;
  stats.totalTaskTime += (elapsedTime / 1000); // en segundos
  
  storage.saveStats(stats);
}