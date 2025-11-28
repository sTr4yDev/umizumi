#include <Arduino.h>

// Configuración
#include "config/pins.h"
#include "config/settings.h"
#include "config/colors.h"

// Hardware
#include "hardware/Display.h"
#include "hardware/Input.h"
#include "hardware/Output.h"
#include "hardware/Sensors.h"
#include "hardware/Storage.h"

// UI
#include "ui/Menu.h"
#include "ui/Screens.h"

// Modos - Productividad
#include "modes/Pomodoro.h"
#include "modes/Gym.h"
#include "modes/productivity/TaskTimer.h"

// TODO: Agregar cuando estén listos
// #include "modes/games/Snake.h"
// #include "modes/tools/Stopwatch.h"
// #include "modes/tools/Countdown.h"
// #include "modes/tools/Flashlight.h"

// ===== ENUMS DE MODOS =====
enum AppMode {
  // Menús
  MODE_MAIN_MENU,
  MODE_PRODUCTIVITY_MENU,
  MODE_GAMES_MENU,
  MODE_TOOLS_MENU,
  MODE_SETTINGS_MENU,
  MODE_INFO_MENU,
  
  // Productividad
  MODE_POMODORO,
  MODE_POMODORO_CONFIG,
  MODE_GYM,
  MODE_TASK_TIMER,
  MODE_STATS,
  
  // Juegos (placeholders)
  MODE_GAME_SNAKE,
  MODE_GAME_PONG,
  MODE_GAME_TETRIS,
  MODE_GAME_FLAPPY,
  MODE_GAME_SPACE_INVADERS,
  
  // Herramientas (placeholders)
  MODE_TOOL_STOPWATCH,
  MODE_TOOL_COUNTDOWN,
  MODE_TOOL_CALCULATOR,
  MODE_TOOL_LEVEL,
  MODE_TOOL_FLASHLIGHT,
  
  // Configuración
  MODE_SET_WIFI,
  MODE_SET_DISPLAY,
  MODE_SET_SOUND,
  MODE_SET_SENSORS,
  MODE_SET_RESET,
  
  // Info
  MODE_INFO_SYSTEM,
  MODE_INFO_WIFI,
  MODE_INFO_GLOBAL_STATS,
  MODE_INFO_ABOUT
};

AppMode currentMode = MODE_MAIN_MENU;
AppMode previousMode = MODE_MAIN_MENU;

unsigned long lastUpdate = 0;
unsigned long lastUIRefresh = 0;
bool longPressHandled = false;

// ===== FUNCIONES AUXILIARES =====

void changeMode(AppMode newMode) {
  Serial.print("Mode change: ");
  Serial.print(currentMode);
  Serial.print(" -> ");
  Serial.println(newMode);
  
  previousMode = currentMode;
  currentMode = newMode;
  
  // Limpiar pantalla
  display.clear();
  
  // Acciones al salir del modo anterior
  switch (previousMode) {
    case MODE_POMODORO:
      pomodoro.stop();
      break;
    case MODE_GYM:
      gym.stop();
      break;
    case MODE_TASK_TIMER:
      // TaskTimer se mantiene corriendo
      break;
    default:
      break;
  }
  
  // Acciones al entrar al nuevo modo
  switch (newMode) {
    case MODE_POMODORO_CONFIG:
      pomodoro.enterConfigMode();
      break;
    case MODE_POMODORO:
      if (previousMode == MODE_POMODORO_CONFIG) {
        pomodoro.exitConfigMode();
      }
      break;
    case MODE_TASK_TIMER:
      if (taskTimer.getState() == TASK_COMPLETED) {
        taskTimer.reset();
      }
      break;
    default:
      break;
  }
  
  // Apagar LEDs al cambiar de modo (excepto TaskTimer)
  if (newMode != MODE_TASK_TIMER || taskTimer.getState() == TASK_IDLE) {
    output.allLedsOff();
  }
}

void handleMenuNavigation() {
  menu.update();
  
  MenuScreen screen = menu.getCurrentScreen();
  static MenuScreen lastScreen = SCREEN_MAIN_MENU;
  
  if (screen != lastScreen) {
    lastScreen = screen;
    
    // Mapear MenuScreen a AppMode
    switch (screen) {
      // Productividad
      case SCREEN_POMODORO_CONFIG:
        changeMode(MODE_POMODORO_CONFIG);
        break;
      case SCREEN_GYM_RUNNING:
        changeMode(MODE_GYM);
        break;
      case SCREEN_TASK_TIMER:
        changeMode(MODE_TASK_TIMER);
        break;
      case SCREEN_STATS:
        changeMode(MODE_STATS);
        break;
        
      // Juegos
      case SCREEN_GAME_SNAKE:
        changeMode(MODE_GAME_SNAKE);
        break;
      case SCREEN_GAME_PONG:
        changeMode(MODE_GAME_PONG);
        break;
      case SCREEN_GAME_TETRIS:
        changeMode(MODE_GAME_TETRIS);
        break;
      case SCREEN_GAME_FLAPPY:
        changeMode(MODE_GAME_FLAPPY);
        break;
      case SCREEN_GAME_SPACE_INVADERS:
        changeMode(MODE_GAME_SPACE_INVADERS);
        break;
        
      // Herramientas
      case SCREEN_TOOL_STOPWATCH:
        changeMode(MODE_TOOL_STOPWATCH);
        break;
      case SCREEN_TOOL_COUNTDOWN:
        changeMode(MODE_TOOL_COUNTDOWN);
        break;
      case SCREEN_TOOL_CALCULATOR:
        changeMode(MODE_TOOL_CALCULATOR);
        break;
      case SCREEN_TOOL_LEVEL:
        changeMode(MODE_TOOL_LEVEL);
        break;
      case SCREEN_TOOL_FLASHLIGHT:
        changeMode(MODE_TOOL_FLASHLIGHT);
        break;
        
      // Configuración
      case SCREEN_SET_WIFI:
        changeMode(MODE_SET_WIFI);
        break;
      case SCREEN_SET_DISPLAY:
        changeMode(MODE_SET_DISPLAY);
        break;
      case SCREEN_SET_SOUND:
        changeMode(MODE_SET_SOUND);
        break;
      case SCREEN_SET_SENSORS:
        changeMode(MODE_SET_SENSORS);
        break;
      case SCREEN_SET_RESET:
        changeMode(MODE_SET_RESET);
        break;
        
      // Info
      case SCREEN_INFO_SYSTEM:
        changeMode(MODE_INFO_SYSTEM);
        break;
      case SCREEN_INFO_WIFI:
        changeMode(MODE_INFO_WIFI);
        break;
      case SCREEN_INFO_GLOBAL_STATS:
        changeMode(MODE_INFO_GLOBAL_STATS);
        break;
      case SCREEN_INFO_ABOUT:
        changeMode(MODE_INFO_ABOUT);
        break;
        
      default:
        break;
    }
  }
}

// ===== HANDLERS DE PRODUCTIVIDAD =====

void handlePomodoroConfigMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_RELEASED:
      pomodoro.nextConfigField();
      break;
      
    case BTN_SELECT_LONG_PRESS:
      pomodoro.saveConfiguration();
      changeMode(MODE_POMODORO);
      break;
      
    case BTN_BACK_RELEASED:
      changeMode(MODE_MAIN_MENU);
      menu.goToScreen(SCREEN_MAIN_MENU);
      break;
      
    default:
      break;
  }
  
  if (input.potChanged()) {
    int potValue = input.getPotPosition();
    pomodoro.adjustConfigValue(potValue);
  }
}

void handlePomodoroMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_RELEASED:
      if (pomodoro.getState() == POM_IDLE) {
        pomodoro.start();
      } else if (pomodoro.getState() == POM_RUNNING) {
        pomodoro.pause();
      } else if (pomodoro.getState() == POM_PAUSED || 
                 pomodoro.getState() == POM_BREAK_PAUSED) {
        pomodoro.resume();
      } else if (pomodoro.getState() == POM_SHORT_BREAK || 
                 pomodoro.getState() == POM_LONG_BREAK) {
        pomodoro.pause();
      }
      break;
      
    case BTN_SELECT_LONG_PRESS:
      if (pomodoro.getState() == POM_IDLE) {
        changeMode(MODE_POMODORO_CONFIG);
      } else {
        pomodoro.stop();
      }
      break;
      
    case BTN_BACK_RELEASED:
      if (pomodoro.getState() == POM_SHORT_BREAK || 
          pomodoro.getState() == POM_LONG_BREAK ||
          pomodoro.getState() == POM_BREAK_PAUSED) {
        pomodoro.skipBreak();
      } else {
        changeMode(MODE_MAIN_MENU);
        menu.goToScreen(SCREEN_MAIN_MENU);
      }
      break;
      
    default:
      break;
  }
  
  pomodoro.update();
}

void handleGymMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_RELEASED:
      if (gym.getState() == GYM_IDLE || gym.getState() == GYM_PAUSED) {
        gym.start();
      } else {
        gym.pause();
      }
      break;
      
    case BTN_SELECT_LONG_PRESS:
      gym.reset();
      break;
      
    case BTN_BACK_RELEASED:
      changeMode(MODE_MAIN_MENU);
      menu.goToScreen(SCREEN_MAIN_MENU);
      break;
      
    default:
      break;
  }
  
  gym.update();
}

void handleTaskTimerMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_RELEASED:
      if (taskTimer.getState() == TASK_IDLE) {
        taskTimer.start();
      } else if (taskTimer.getState() == TASK_RUNNING) {
        taskTimer.pause();
      } else if (taskTimer.getState() == TASK_PAUSED) {
        taskTimer.resume();
      } else if (taskTimer.getState() == TASK_COMPLETED) {
        taskTimer.reset();
      }
      break;
      
    case BTN_SELECT_LONG_PRESS:
      if (taskTimer.getState() == TASK_RUNNING || 
          taskTimer.getState() == TASK_PAUSED) {
        taskTimer.stop();
      }
      break;
      
    case BTN_BACK_RELEASED:
      changeMode(MODE_MAIN_MENU);
      menu.goToScreen(SCREEN_MAIN_MENU);
      break;
      
    default:
      break;
  }
  
  taskTimer.update();
}

void handleStatsMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_LONG_PRESS:
      if (!longPressHandled) {
        storage.resetStats();
        output.playSuccess();
        delay(500);
        
        Statistics stats;
        storage.loadStats(stats);
        
        longPressHandled = true;
        
        Serial.println("📊 Statistics reset!");
      }
      break;
      
    case BTN_BACK_RELEASED:
      changeMode(MODE_MAIN_MENU);
      menu.goToScreen(SCREEN_MAIN_MENU);
      longPressHandled = false;
      break;
      
    default:
      longPressHandled = false;
      break;
  }
}

// ===== HANDLERS DE PLACEHOLDERS =====

void handlePlaceholder(const char* feature) {
  display.clear();
  
  display.fillRect(0, 0, display.getWidth(), 22, COLOR_WARNING);
  display.drawCentered("EN DESARROLLO", 7, ST77XX_WHITE, 1);
  
  display.drawCentered(feature, 50, COLOR_TEXT, 2);
  display.drawCentered("Proximamente...", 80, COLOR_INFO, 1);
  
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
  
  ButtonEvent event = input.getButtonEvent();
  if (event == BTN_BACK_RELEASED) {
    changeMode(MODE_MAIN_MENU);
    menu.goToScreen(SCREEN_MAIN_MENU);
  }
}

// ===== UPDATE UI =====

void updateUI() {
  unsigned long now = millis();
  
  if (now - lastUIRefresh < UI_REFRESH_MS) {
    return;
  }
  lastUIRefresh = now;
  
  switch (currentMode) {
    // Menús
    case MODE_MAIN_MENU:
    case MODE_PRODUCTIVITY_MENU:
    case MODE_GAMES_MENU:
    case MODE_TOOLS_MENU:
    case MODE_SETTINGS_MENU:
    case MODE_INFO_MENU:
      menu.draw();
      break;
      
    // Productividad
    case MODE_POMODORO:
    case MODE_POMODORO_CONFIG:
      pomodoro.draw();
      break;
      
    case MODE_GYM:
      gym.draw();
      break;
      
    case MODE_TASK_TIMER:
      taskTimer.draw();
      break;
      
    case MODE_STATS: {
      Statistics stats;
      storage.loadStats(stats);
      Screens::drawStats(stats);
      break;
    }
      
    // Juegos (placeholders)
    case MODE_GAME_SNAKE:
      handlePlaceholder("SNAKE");
      break;
    case MODE_GAME_PONG:
      handlePlaceholder("PONG");
      break;
    case MODE_GAME_TETRIS:
      handlePlaceholder("TETRIS");
      break;
    case MODE_GAME_FLAPPY:
      handlePlaceholder("FLAPPY BIRD");
      break;
    case MODE_GAME_SPACE_INVADERS:
      handlePlaceholder("SPACE INVADERS");
      break;
      
    // Herramientas (placeholders)
    case MODE_TOOL_STOPWATCH:
      handlePlaceholder("CRONOMETRO");
      break;
    case MODE_TOOL_COUNTDOWN:
      handlePlaceholder("CUENTA REGRESIVA");
      break;
    case MODE_TOOL_CALCULATOR:
      handlePlaceholder("CALCULADORA");
      break;
    case MODE_TOOL_LEVEL:
      handlePlaceholder("NIVEL");
      break;
    case MODE_TOOL_FLASHLIGHT:
      handlePlaceholder("LINTERNA");
      break;
      
    // Configuración (placeholders)
    case MODE_SET_WIFI:
      handlePlaceholder("CONFIG WIFI");
      break;
    case MODE_SET_DISPLAY:
      handlePlaceholder("CONFIG DISPLAY");
      break;
    case MODE_SET_SOUND:
      handlePlaceholder("CONFIG SONIDO");
      break;
    case MODE_SET_SENSORS:
      handlePlaceholder("CONFIG SENSORES");
      break;
    case MODE_SET_RESET:
      handlePlaceholder("RESET");
      break;
      
    // Info
    case MODE_INFO_SYSTEM:
      Screens::drawInfo();
      if (input.getButtonEvent() == BTN_BACK_RELEASED) {
        changeMode(MODE_MAIN_MENU);
        menu.goToScreen(SCREEN_MAIN_MENU);
      }
      break;
      
    case MODE_INFO_WIFI:
      handlePlaceholder("WIFI STATUS");
      break;
      
    case MODE_INFO_GLOBAL_STATS: {
      Statistics stats;
      storage.loadStats(stats);
      Screens::drawStats(stats);
      if (input.getButtonEvent() == BTN_BACK_RELEASED) {
        changeMode(MODE_MAIN_MENU);
        menu.goToScreen(SCREEN_MAIN_MENU);
      }
      break;
    }
      
    case MODE_INFO_ABOUT:
      handlePlaceholder("ACERCA DE");
      break;
      
    default:
      break;
  }
}

// ===== SETUP =====

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n\n");
  Serial.println("================================");
  Serial.println("   POMODORO TIMER v2.5");
  Serial.println("   ESP32 Modular System");
  Serial.println("   New Menu Structure");
  Serial.println("================================");
  Serial.println();
  
  // Inicializar hardware
  Serial.println("Initializing hardware...");
  
  display.begin();
  
  // Test visual
  display.fillScreen(ST77XX_GREEN);
  delay(300);
  display.fillScreen(ST77XX_BLACK);
  
  input.begin();
  output.begin();
  sensors.begin();
  storage.begin();
  
  // Inicializar UI
  Serial.println("Initializing UI...");
  menu.begin();
  
  // Inicializar modos de productividad
  Serial.println("Initializing productivity modes...");
  pomodoro.begin();
  gym.begin();
  taskTimer.begin();
  
  // Pantalla de bienvenida
  display.clear();
  display.drawCentered("POMODORO", 35, COLOR_PRIMARY, 3);
  display.drawCentered("TIMER", 60, COLOR_PRIMARY, 3);
  display.drawCentered("v2.5", 90, COLOR_TEXT, 1);
  display.drawCentered("New Menu!", 105, COLOR_SUCCESS, 1);
  
  // Efectos
  output.setGreenLED(LED_BLINK_FAST);
  output.playSuccess();
  delay(2500);
  output.setGreenLED(LED_OFF);
  
  // Ir al menú
  display.clear();
  changeMode(MODE_MAIN_MENU);
  
  Serial.println();
  Serial.println("✅ System ready!");
  Serial.println("================================");
  Serial.println();
}

// ===== LOOP =====

void loop() {
  unsigned long now = millis();
  
  // Actualizar inputs
  input.update();
  sensors.update();
  output.update();
  
  // Manejar modo actual
  switch (currentMode) {
    // Menús
    case MODE_MAIN_MENU:
    case MODE_PRODUCTIVITY_MENU:
    case MODE_GAMES_MENU:
    case MODE_TOOLS_MENU:
    case MODE_SETTINGS_MENU:
    case MODE_INFO_MENU:
      handleMenuNavigation();
      break;
      
    // Productividad
    case MODE_POMODORO:
      handlePomodoroMode();
      break;
    case MODE_POMODORO_CONFIG:
      handlePomodoroConfigMode();
      break;
    case MODE_GYM:
      handleGymMode();
      break;
    case MODE_TASK_TIMER:
      handleTaskTimerMode();
      break;
    case MODE_STATS:
      handleStatsMode();
      break;
      
    default:
      // Los placeholders se manejan en updateUI()
      break;
  }
  
  // Actualizar UI
  updateUI();
  
  delay(10);
}