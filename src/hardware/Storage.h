#ifndef STORAGE_H
#define STORAGE_H

#include <Preferences.h>
#include "../config/settings.h"

struct PomodoroConfig {
  uint16_t workTime;      // Segundos
  uint16_t shortBreak;    // Segundos
  uint16_t longBreak;     // Segundos
  uint8_t cyclesUntilLong;
};

struct GymConfig {
  uint16_t minSerieDuration; // Milisegundos
  uint16_t targetSeries;
};

struct Statistics {
  uint32_t pomodorosCompleted;
  uint32_t gymSeriesCompleted;
  uint32_t soundInterruptions;
  uint32_t totalWorkTime;      // Segundos totales trabajados
};

class Storage {
public:
  void begin();
  
  // Pomodoro
  void loadPomodoroConfig(PomodoroConfig &config);
  void savePomodoroConfig(const PomodoroConfig &config);
  
  // Gym
  void loadGymConfig(GymConfig &config);
  void saveGymConfig(const GymConfig &config);
  
  // Estadísticas
  void loadStats(Statistics &stats);
  void saveStats(const Statistics &stats);
  void incrementPomodoros();
  void incrementGymSeries();
  void incrementInterruptions();
  void addWorkTime(uint32_t seconds);
  void resetStats();
  
private:
  Preferences prefs;
};

extern Storage storage;

#endif