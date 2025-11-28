#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <Preferences.h>

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
  // Pomodoro
  unsigned long pomodorosCompleted;
  unsigned long totalWorkTime;        // segundos
  unsigned long totalBreakTime;       // segundos
  
  // Gym
  unsigned long gymSessions;
  unsigned long totalGymTime;         // segundos
  unsigned long gymSeriesCompleted;
  unsigned long soundInterruptions;
  
  // TaskTimer
  unsigned long tasksCompleted;
  unsigned long totalTaskTime;        // segundos
  
  // General
  unsigned long lastUpdate;
};

class Storage {
public:
  void begin();
  
  // Pomodoro Configuration
  void loadPomodoroConfig(PomodoroConfig &config);
  void savePomodoroConfig(const PomodoroConfig &config);
  
  // Gym Configuration  
  void loadGymConfig(GymConfig &config);
  void saveGymConfig(const GymConfig &config);
  
  // Alternative Pomodoro config methods (for compatibility)
  void savePomodoroConfig(int work, int shortBreak, int longBreak);
  void loadPomodoroConfig(int& work, int& shortBreak, int& longBreak);
  
  // Statistics
  void loadStats(Statistics &stats);
  void saveStats(Statistics &stats);
  void resetStats();
  
  // Statistics increment methods
  void incrementPomodoros();
  void incrementGymSeries();
  void incrementInterruptions();
  void addWorkTime(uint32_t seconds);
  
private:
  Preferences prefs;
};

extern Storage storage;

#endif