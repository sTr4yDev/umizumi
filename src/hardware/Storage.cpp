// src/hardware/Storage.cpp

#include "Storage.h"

// Instancia global
Storage storage;

void Storage::begin() {
  prefs.begin("umizumi", false);
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

// === GYM CONFIG ===
void Storage::loadGymConfig(GymConfig &config) {
  config.minSerieDuration = prefs.getUShort("gym_duration", 1000);
  config.targetSeries = prefs.getUShort("gym_target", 10);
}

void Storage::saveGymConfig(const GymConfig &config) {
  prefs.putUShort("gym_duration", config.minSerieDuration);
  prefs.putUShort("gym_target", config.targetSeries);
}

// === ESTADÍSTICAS ===
void Storage::loadStats(Statistics &stats) {
  stats.pomodorosCompleted = prefs.getUInt("stat_pomos", 0);
  stats.gymSeriesCompleted = prefs.getUInt("stat_gym", 0);
  stats.soundInterruptions = prefs.getUInt("stat_sound", 0);
  stats.totalWorkTime = prefs.getUInt("stat_time", 0);
}

void Storage::saveStats(const Statistics &stats) {
  prefs.putUInt("stat_pomos", stats.pomodorosCompleted);
  prefs.putUInt("stat_gym", stats.gymSeriesCompleted);
  prefs.putUInt("stat_sound", stats.soundInterruptions);
  prefs.putUInt("stat_time", stats.totalWorkTime);
}

void Storage::incrementPomodoros() {
  uint32_t count = prefs.getUInt("stat_pomos", 0);
  prefs.putUInt("stat_pomos", count + 1);
}

void Storage::incrementGymSeries() {
  uint32_t count = prefs.getUInt("stat_gym", 0);
  prefs.putUInt("stat_gym", count + 1);
}

void Storage::incrementInterruptions() {
  uint32_t count = prefs.getUInt("stat_sound", 0);
  prefs.putUInt("stat_sound", count + 1);
}

void Storage::addWorkTime(uint32_t seconds) {
  uint32_t total = prefs.getUInt("stat_time", 0);
  prefs.putUInt("stat_time", total + seconds);
}

void Storage::resetStats() {
  prefs.putUInt("stat_pomos", 0);
  prefs.putUInt("stat_gym", 0);
  prefs.putUInt("stat_sound", 0);
  prefs.putUInt("stat_time", 0);
}