#include "Screens.h"

void Screens::drawPomodoroConfig(PomodoroConfig &config, int selectedField) {
  display.clear();
  
  // Título
  display.drawCentered("CONFIGURAR", 8, COLOR_PRIMARY, 2);
  display.drawCentered("POMODORO", 24, COLOR_PRIMARY, 2);
  
  // Línea
  display.drawLine(10, 42, display.getWidth() - 10, 42, COLOR_PRIMARY);
  
  char line[32];
  
  // Trabajo
  snprintf(line, sizeof(line), "Trabajo: %02d:%02d", config.workTime / 60, config.workTime % 60);
  if (selectedField == 0) {
    display.fillRect(8, 48, display.getWidth() - 16, 14, COLOR_MENU_SELECT);
    display.drawText(line, 12, 50, COLOR_BG, 1);
  } else {
    display.drawText(line, 12, 50, COLOR_TEXT, 1);
  }
  
  // Break corto
  snprintf(line, sizeof(line), "Break corto: %02d:%02d", config.shortBreak / 60, config.shortBreak % 60);
  if (selectedField == 1) {
    display.fillRect(8, 66, display.getWidth() - 16, 14, COLOR_MENU_SELECT);
    display.drawText(line, 12, 68, COLOR_BG, 1);
  } else {
    display.drawText(line, 12, 68, COLOR_TEXT, 1);
  }
  
  // Break largo
  snprintf(line, sizeof(line), "Break largo: %02d:%02d", config.longBreak / 60, config.longBreak % 60);
  if (selectedField == 2) {
    display.fillRect(8, 84, display.getWidth() - 16, 14, COLOR_MENU_SELECT);
    display.drawText(line, 12, 86, COLOR_BG, 1);
  } else {
    display.drawText(line, 12, 86, COLOR_TEXT, 1);
  }
  
  // Ciclos
  snprintf(line, sizeof(line), "Ciclos: %d", config.cyclesUntilLong);
  if (selectedField == 3) {
    display.fillRect(8, 102, display.getWidth() - 16, 14, COLOR_MENU_SELECT);
    display.drawText(line, 12, 104, COLOR_BG, 1);
  } else {
    display.drawText(line, 12, 104, COLOR_TEXT, 1);
  }
  
  // Ayuda
  display.drawText("POT: Ajustar", 4, display.getHeight() - 10, COLOR_INFO, 1);
  display.drawRightAligned("BACK: Salir", display.getHeight() - 10, COLOR_INFO, 1);
}

void Screens::drawPomodoroRunning(const char* state, unsigned long timeLeft, uint32_t pomodorosCount) {
  display.clear();
  
  // Título
  display.drawCentered("POMODORO", 8, COLOR_PRIMARY, 2);
  
  // Estado
  uint16_t stateColor = COLOR_IDLE;
  if (strcmp(state, "Work") == 0) stateColor = COLOR_RUNNING;
  else if (strcmp(state, "Paused") == 0) stateColor = COLOR_PAUSED;
  else if (strcmp(state, "Break") == 0) stateColor = COLOR_BREAK;
  
  display.drawCentered(state, 30, stateColor, 2);
  
  // Timer grande
  char timerStr[16];
  int minutes = timeLeft / 60;
  int seconds = timeLeft % 60;
  snprintf(timerStr, sizeof(timerStr), "%02d:%02d", minutes, seconds);
  display.drawCentered(timerStr, 55, COLOR_TEXT, 3);
  
  // Barra de progreso (ejemplo: 25min = 100%)
  int totalTime = 25 * 60; // Asumiendo 25 min por defecto
  int percent = 100 - ((timeLeft * 100) / totalTime);
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
  
  drawProgressBar(10, 90, display.getWidth() - 20, 8, percent, COLOR_SUCCESS);
  
  // Contador
  char countStr[32];
  snprintf(countStr, sizeof(countStr), "Completados: %lu", pomodorosCount);
  display.drawCentered(countStr, 110, COLOR_TEXT, 1);
  
  // Controles
  display.drawText("SELECT: Start/Pause", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

void Screens::drawGymRunning(unsigned long elapsed, uint32_t seriesCount) {
  display.clear();
  
  // Título
  display.drawCentered("GYM MODE", 8, COLOR_PRIMARY, 2);
  
  // Cronómetro
  char timeStr[16];
  int hours = elapsed / 3600;
  int minutes = (elapsed % 3600) / 60;
  int seconds = elapsed % 60;
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
  display.drawCentered(timeStr, 45, COLOR_RUNNING, 3);
  
  // Series
  char seriesStr[32];
  snprintf(seriesStr, sizeof(seriesStr), "Series: %lu", seriesCount);
  display.drawCentered(seriesStr, 85, COLOR_TEXT, 2);
  
  // Instrucciones
  display.drawText("CLAP: Start/Stop", 8, 110, COLOR_INFO, 1);
  display.drawText("BACK: Menu", 8, 122, COLOR_INFO, 1);
}

void Screens::drawStats(const Statistics &stats) {
  display.clear();
  
  // Título
  display.drawCentered("ESTADISTICAS", 8, COLOR_PRIMARY, 2);
  display.drawLine(10, 26, display.getWidth() - 10, 26, COLOR_PRIMARY);
  
  char line[32];
  
  // Pomodoros
  snprintf(line, sizeof(line), "Pomodoros: %lu", stats.pomodorosCompleted);
  display.drawText(line, 8, 36, COLOR_TEXT, 1);
  
  // Gym series
  snprintf(line, sizeof(line), "Series Gym: %lu", stats.gymSeriesCompleted);
  display.drawText(line, 8, 52, COLOR_TEXT, 1);
  
  // Interrupciones
  snprintf(line, sizeof(line), "Interrupciones: %lu", stats.soundInterruptions);
  display.drawText(line, 8, 68, COLOR_TEXT, 1);
  
  // Tiempo total trabajado
  uint32_t hours = stats.totalWorkTime / 3600;
  uint32_t minutes = (stats.totalWorkTime % 3600) / 60;
  snprintf(line, sizeof(line), "Tiempo total: %luh %lum", hours, minutes);
  display.drawText(line, 8, 84, COLOR_TEXT, 1);
  
  // Botón para resetear
  display.drawCentered("LONG PRESS: Reset", 110, COLOR_DANGER, 1);
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

void Screens::drawSettings() {
  display.clear();
  
  display.drawCentered("CONFIGURACION", 8, COLOR_PRIMARY, 2);
  display.drawLine(10, 26, display.getWidth() - 10, 26, COLOR_PRIMARY);
  
  display.drawText("WiFi: Desactivado", 8, 40, COLOR_TEXT, 1);
  display.drawText("Buzzer: Activado", 8, 56, COLOR_TEXT, 1);
  display.drawText("Sensor: Activado", 8, 72, COLOR_TEXT, 1);
  
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

void Screens::drawInfo() {
  display.clear();
  
  display.drawCentered("INFO", 8, COLOR_PRIMARY, 2);
  display.drawLine(10, 26, display.getWidth() - 10, 26, COLOR_PRIMARY);
  
  display.drawText("Pomodoro Timer", 8, 40, COLOR_TEXT, 1);
  display.drawText("Version: 2.0.0", 8, 56, COLOR_TEXT, 1);
  display.drawText("Hardware: ESP32", 8, 72, COLOR_TEXT, 1);
  
  char uptime[32];
  unsigned long seconds = millis() / 1000;
  unsigned long hours = seconds / 3600;
  unsigned long minutes = (seconds % 3600) / 60;
  snprintf(uptime, sizeof(uptime), "Uptime: %luh %lum", hours, minutes);
  display.drawText(uptime, 8, 88, COLOR_TEXT, 1);
  
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

void Screens::drawProgressBar(int x, int y, int width, int height, int percent, uint16_t color) {
  // Borde
  display.drawRect(x, y, width, height, COLOR_TEXT);
  
  // Relleno
  int fillWidth = (width - 2) * percent / 100;
  if (fillWidth > 0) {
    display.fillRect(x + 1, y + 1, fillWidth, height - 2, color);
  }
  
  // Porcentaje
  char percentStr[8];
  snprintf(percentStr, sizeof(percentStr), "%d%%", percent);
  
  int16_t x1, y1;
  uint16_t w, h;
  display.getTFT()->getTextBounds(percentStr, 0, 0, &x1, &y1, &w, &h);
  
  int textX = x + (width - w) / 2;
  int textY = y + height + 2;
  
  display.drawText(percentStr, textX, textY, COLOR_TEXT, 1);
}

void Screens::drawTimeString(char* buffer, unsigned long seconds) {
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int secs = seconds % 60;
  
  if (hours > 0) {
    snprintf(buffer, 16, "%02d:%02d:%02d", hours, minutes, secs);
  } else {
    snprintf(buffer, 16, "%02d:%02d", minutes, secs);
  }
}