#include "Storage.h"
#include "../config/settings.h"

// Instancia global
Storage storage;

void Storage::begin() {
  prefs.begin("umizumi", false);
  Serial.println("Storage initialized");
}

// === POMODORO CONFIG ===
void Storage::loadPomodoroConfig(PomodoroConfig &config) {
  config.workTime = prefs.getUShort("pomo_work", DEFAULT_POMODORO_TIME);
  config.shortBreak = prefs.getUShort("pomo_short", DEFAULT_SHORT_BREAK);
  config.longBreak = prefs.getUShort("pomo_long", DEFAULT_LONG_BREAK);
  config.cyclesUntilLong = prefs.getUChar("pomo_cycles", POMODOROS_UNTIL_LONG);
}

void Storage::savePomodoroConfig(const PomodoroConfig &config) {
  prefs.putUShort("pomo_work", config.workTime);
  prefs.putUShort("pomo_short", config.shortBreak);
  prefs.putUShort("pomo_long", config.longBreak);
  prefs.putUChar("pomo_cycles", config.cyclesUntilLong);
}

// Métodos alternativos para compatibilidad
void Storage::savePomodoroConfig(int work, int shortBreak, int longBreak) {
  PomodoroConfig config;
  config.workTime = work;
  config.shortBreak = shortBreak;
  config.longBreak = longBreak;
  config.cyclesUntilLong = POMODOROS_UNTIL_LONG;
  savePomodoroConfig(config);
}

void Storage::loadPomodoroConfig(int& work, int& shortBreak, int& longBreak) {
  PomodoroConfig config;
  loadPomodoroConfig(config);
  work = config.workTime;
  shortBreak = config.shortBreak;
  longBreak = config.longBreak;
}

// === GYM CONFIG ===
void Storage::loadGymConfig(GymConfig &config) {
  config.minSerieDuration = prefs.getUShort("gym_duration", DEFAULT_GYM_SERIE_DURATION);
  config.targetSeries = prefs.getUShort("gym_target", DEFAULT_GYM_TARGET_SERIES);
}

void Storage::saveGymConfig(const GymConfig &config) {
  prefs.putUShort("gym_duration", config.minSerieDuration);
  prefs.putUShort("gym_target", config.targetSeries);
}

// === ESTADÍSTICAS COMPLETAS ===
void Storage::loadStats(Statistics &stats) {
  // Pomodoro - ⭐ USAR getULong para unsigned long
  stats.pomodorosCompleted = prefs.getULong("stat_pomos", 0);
  stats.totalWorkTime = prefs.getULong("stat_work_time", 0);
  stats.totalBreakTime = prefs.getULong("stat_break_time", 0);
  
  // Gym - ⭐ USAR getULong para unsigned long
  stats.gymSessions = prefs.getULong("stat_gym_sessions", 0);
  stats.totalGymTime = prefs.getULong("stat_gym_time", 0);
  stats.gymSeriesCompleted = prefs.getULong("stat_gym_series", 0);
  stats.soundInterruptions = prefs.getULong("stat_sound", 0);
  
  // TaskTimer - ⭐ USAR getULong para unsigned long
  stats.tasksCompleted = prefs.getULong("stat_tasks", 0);
  stats.totalTaskTime = prefs.getULong("stat_task_time", 0);
  
  // General
  stats.lastUpdate = prefs.getULong("stat_last_update", 0);
}

void Storage::saveStats(Statistics &stats) {
  // Pomodoro - ⭐ USAR putULong para unsigned long
  prefs.putULong("stat_pomos", stats.pomodorosCompleted);
  prefs.putULong("stat_work_time", stats.totalWorkTime);
  prefs.putULong("stat_break_time", stats.totalBreakTime);
  
  // Gym - ⭐ USAR putULong para unsigned long
  prefs.putULong("stat_gym_sessions", stats.gymSessions);
  prefs.putULong("stat_gym_time", stats.totalGymTime);
  prefs.putULong("stat_gym_series", stats.gymSeriesCompleted);
  prefs.putULong("stat_sound", stats.soundInterruptions);
  
  // TaskTimer - ⭐ USAR putULong para unsigned long
  prefs.putULong("stat_tasks", stats.tasksCompleted);
  prefs.putULong("stat_task_time", stats.totalTaskTime);
  
  // General
  prefs.putULong("stat_last_update", stats.lastUpdate);
}

void Storage::resetStats() {
  // Pomodoro - ⭐ USAR putULong para unsigned long
  prefs.putULong("stat_pomos", 0);
  prefs.putULong("stat_work_time", 0);
  prefs.putULong("stat_break_time", 0);
  
  // Gym - ⭐ USAR putULong para unsigned long
  prefs.putULong("stat_gym_sessions", 0);
  prefs.putULong("stat_gym_time", 0);
  prefs.putULong("stat_gym_series", 0);
  prefs.putULong("stat_sound", 0);
  
  // TaskTimer - ⭐ USAR putULong para unsigned long
  prefs.putULong("stat_tasks", 0);
  prefs.putULong("stat_task_time", 0);
  
  // General
  prefs.putULong("stat_last_update", millis() / 1000);
  
  Serial.println("Statistics reset");
}

// === MÉTODOS DE INCREMENTO ===
void Storage::incrementPomodoros() {
  Statistics stats;
  loadStats(stats);
  stats.pomodorosCompleted++;
  stats.lastUpdate = millis() / 1000;
  saveStats(stats);
}

void Storage::incrementGymSeries() {
  Statistics stats;
  loadStats(stats);
  stats.gymSeriesCompleted++;
  stats.lastUpdate = millis() / 1000;
  saveStats(stats);
}

void Storage::incrementInterruptions() {
  Statistics stats;
  loadStats(stats);
  stats.soundInterruptions++;
  stats.lastUpdate = millis() / 1000;
  saveStats(stats);
}

void Storage::addWorkTime(uint32_t seconds) {
  Statistics stats;
  loadStats(stats);
  stats.totalWorkTime += seconds;
  stats.lastUpdate = millis() / 1000;
  saveStats(stats);
}