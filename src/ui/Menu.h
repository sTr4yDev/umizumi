#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "../hardware/Display.h"
#include "../hardware/Input.h"
#include "../hardware/Output.h"
#include "../config/colors.h"

// ===== ENUMS =====

// Menú Principal
enum MainMenuOption {
  MAIN_PRODUCTIVITY = 0,
  MAIN_GAMES,
  MAIN_TOOLS,
  MAIN_SETTINGS,
  MAIN_INFO,
  MAIN_OPTION_COUNT
};

// Submenú Productividad
enum ProductivityMenuOption {
  PROD_POMODORO = 0,
  PROD_GYM,
  PROD_TASK_TIMER,
  PROD_STATS,
  PROD_OPTION_COUNT
};

// Submenú Juegos
enum GamesMenuOption {
  GAME_SNAKE = 0,
  GAME_PONG,
  GAME_TETRIS,
  GAME_FLAPPY,
  GAME_SPACE_INVADERS,
  GAME_OPTION_COUNT
};

// Submenú Herramientas
enum ToolsMenuOption {
  TOOL_STOPWATCH = 0,
  TOOL_COUNTDOWN,
  TOOL_CALCULATOR,
  TOOL_LEVEL,
  TOOL_FLASHLIGHT,
  TOOL_OPTION_COUNT
};

// Submenú Configuración
enum SettingsMenuOption {
  SET_WIFI = 0,
  SET_DISPLAY,
  SET_SOUND,
  SET_SENSORS,
  SET_RESET,
  SET_OPTION_COUNT
};

// Submenú Info
enum InfoMenuOption {
  INFO_SYSTEM = 0,
  INFO_WIFI_STATUS,
  INFO_GLOBAL_STATS,
  INFO_ABOUT,
  INFO_OPTION_COUNT
};

// Screens
enum MenuScreen {
  SCREEN_MAIN_MENU,
  
  // Productividad
  SCREEN_PRODUCTIVITY_MENU,
  SCREEN_POMODORO_CONFIG,
  SCREEN_POMODORO_RUNNING,
  SCREEN_GYM_RUNNING,
  SCREEN_TASK_TIMER,
  SCREEN_STATS,
  
  // Juegos
  SCREEN_GAMES_MENU,
  SCREEN_GAME_SNAKE,
  SCREEN_GAME_PONG,
  SCREEN_GAME_TETRIS,
  SCREEN_GAME_FLAPPY,
  SCREEN_GAME_SPACE_INVADERS,
  
  // Herramientas
  SCREEN_TOOLS_MENU,
  SCREEN_TOOL_STOPWATCH,
  SCREEN_TOOL_COUNTDOWN,
  SCREEN_TOOL_CALCULATOR,
  SCREEN_TOOL_LEVEL,
  SCREEN_TOOL_FLASHLIGHT,
  
  // Configuración
  SCREEN_SETTINGS_MENU,
  SCREEN_SET_WIFI,
  SCREEN_SET_DISPLAY,
  SCREEN_SET_SOUND,
  SCREEN_SET_SENSORS,
  SCREEN_SET_RESET,
  
  // Info
  SCREEN_INFO_MENU,
  SCREEN_INFO_SYSTEM,
  SCREEN_INFO_WIFI,
  SCREEN_INFO_GLOBAL_STATS,
  SCREEN_INFO_ABOUT
};

// ===== CLASE MENU =====

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
  void drawProductivityMenu();
  void drawGamesMenu();
  void drawToolsMenu();
  void drawSettingsMenu();
  void drawInfoMenu();
  
  void drawMenuOption(const char* text, int index, bool selected, int yOffset = 40);
  void drawHeader(const char* title, uint16_t color);
};

extern Menu menu;

#endif