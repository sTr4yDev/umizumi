#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "../hardware/Display.h"
#include "../hardware/Input.h"
#include "../hardware/Output.h"
#include "../config/colors.h"

enum MenuScreen {
  SCREEN_MAIN_MENU,
  SCREEN_POMODORO_RUNNING,
  SCREEN_POMODORO_CONFIG,
  SCREEN_GYM_RUNNING,
  SCREEN_GYM_CONFIG,
  SCREEN_SETTINGS,
  SCREEN_STATS,
  SCREEN_INFO
};

enum MainMenuOption {
  MENU_POMODORO = 0,
  MENU_GYM = 1,
  MENU_SETTINGS = 2,
  MENU_STATS = 3,
  MENU_INFO = 4,
  MENU_OPTION_COUNT = 5
};

class Menu {
public:
  void begin();
  void update();
  void draw();
  
  // Navegación
  void goToScreen(MenuScreen screen);
  MenuScreen getCurrentScreen();
  int getSelectedOption();
  
  // Control
  void handlePotChange(int value);
  void handleSelectButton();
  void handleBackButton();
  
private:
  MenuScreen currentScreen;
  MenuScreen previousScreen;
  int selectedOption;
  int lastPotValue;
  bool needsRedraw;
  
  // Dibujar menús
  void drawMainMenu();
  void drawMenuOption(const char* text, int index, bool selected);
};

extern Menu menu;

#endif