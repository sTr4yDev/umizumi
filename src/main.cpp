//Hola 
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

// Modos
#include "modes/Pomodoro.h"
#include "modes/Gym.h"

// ===== VARIABLES GLOBALES =====
enum AppMode {
  MODE_MENU,
  MODE_POMODORO,
  MODE_POMODORO_CONFIG,
  MODE_GYM,
  MODE_SETTINGS,
  MODE_STATS,
  MODE_INFO
};

AppMode currentMode = MODE_MENU;
AppMode previousMode = MODE_MENU;

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
  
  // Limpiar pantalla en cambio de modo
  display.clear();
  
  // Acciones al entrar en cada modo
  switch (newMode) {
    case MODE_POMODORO_CONFIG:
      pomodoro.enterConfigMode();
      break;
      
    case MODE_POMODORO:
      if (previousMode == MODE_POMODORO_CONFIG) {
        pomodoro.exitConfigMode();
      }
      break;
      
    case MODE_GYM:
      // Resetear gym si viene del menú
      if (previousMode == MODE_MENU) {
        gym.stop();
      }
      break;
      
    case MODE_MENU:
      // Detener todo al volver al menú
      if (previousMode == MODE_POMODORO) {
        pomodoro.stop();
      }
      if (previousMode == MODE_GYM) {
        gym.stop();
      }
      output.allLedsOff();
      break;
      
    default:
      break;
  }
}

void handleMenuNavigation() {
  // El menú maneja su propia navegación
  menu.update();
  
  // Detectar si se seleccionó una opción
  MenuScreen screen = menu.getCurrentScreen();
  
  static MenuScreen lastScreen = SCREEN_MAIN_MENU;
  
  if (screen != lastScreen) {
    lastScreen = screen;
    
    // Cambiar modo según pantalla seleccionada
    switch (screen) {
      case SCREEN_POMODORO_CONFIG:
        changeMode(MODE_POMODORO_CONFIG);
        break;
        
      case SCREEN_GYM_RUNNING:
        changeMode(MODE_GYM);
        break;
        
      case SCREEN_SETTINGS:
        changeMode(MODE_SETTINGS);
        break;
        
      case SCREEN_STATS:
        changeMode(MODE_STATS);
        break;
        
      case SCREEN_INFO:
        changeMode(MODE_INFO);
        break;
        
      default:
        break;
    }
  }
}

void handlePomodoroConfigMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_RELEASED:
      // Siguiente campo
      pomodoro.nextConfigField();
      break;
      
    case BTN_SELECT_LONG_PRESS:
      // Guardar y salir
      pomodoro.saveConfiguration();
      changeMode(MODE_POMODORO);
      break;
      
    case BTN_BACK_RELEASED:
      // Salir sin guardar
      changeMode(MODE_MENU);
      menu.goToScreen(SCREEN_MAIN_MENU);
      break;
      
    default:
      break;
  }
  
  // Ajustar valores con potenciómetro
  if (input.potChanged()) {
    int potValue = input.getPotPosition();
    pomodoro.adjustConfigValue(potValue);
  }
}

void handlePomodoroMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_RELEASED:
      // Toggle Start/Pause
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
      // Long press: Entrar a configuración
      if (pomodoro.getState() == POM_IDLE) {
        changeMode(MODE_POMODORO_CONFIG);
      } else {
        // Si está corriendo, detener
        pomodoro.stop();
      }
      break;
      
    case BTN_BACK_RELEASED:
      // Skip break o volver al menú
      if (pomodoro.getState() == POM_SHORT_BREAK || 
          pomodoro.getState() == POM_LONG_BREAK ||
          pomodoro.getState() == POM_BREAK_PAUSED) {
        pomodoro.skipBreak();
      } else {
        changeMode(MODE_MENU);
        menu.goToScreen(SCREEN_MAIN_MENU);
      }
      break;
      
    default:
      break;
  }
  
  // Actualizar lógica del pomodoro
  pomodoro.update();
}

void handleGymMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_RELEASED:
      // Manual start/pause (alternativo al CLAP)
      if (gym.getState() == GYM_IDLE || gym.getState() == GYM_PAUSED) {
        gym.start();
      } else {
        gym.pause();
      }
      break;
      
    case BTN_SELECT_LONG_PRESS:
      // Reset session
      gym.reset();
      break;
      
    case BTN_BACK_RELEASED:
      // Volver al menú
      changeMode(MODE_MENU);
      menu.goToScreen(SCREEN_MAIN_MENU);
      break;
      
    default:
      break;
  }
  
  // Actualizar lógica del gym
  gym.update();
}

void handleStatsMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_LONG_PRESS:
      // Resetear estadísticas
      if (!longPressHandled) {
        storage.resetStats();
        output.playSuccess();
        delay(500);
        
        // Recargar stats
        Statistics stats;
        storage.loadStats(stats);
        
        longPressHandled = true;
        
        Serial.println("📊 Statistics reset!");
      }
      break;
      
    case BTN_BACK_RELEASED:
      changeMode(MODE_MENU);
      menu.goToScreen(SCREEN_MAIN_MENU);
      longPressHandled = false;
      break;
      
    default:
      longPressHandled = false;
      break;
  }
}

void handleSettingsMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_BACK_RELEASED:
      changeMode(MODE_MENU);
      menu.goToScreen(SCREEN_MAIN_MENU);
      break;
      
    default:
      break;
  }
}

void handleInfoMode() {
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_BACK_RELEASED:
      changeMode(MODE_MENU);
      menu.goToScreen(SCREEN_MAIN_MENU);
      break;
      
    default:
      break;
  }
}

void updateUI() {
  unsigned long now = millis();
  
  // Refrescar UI cada 300ms
  if (now - lastUIRefresh < UI_REFRESH_MS) {
    return;
  }
  lastUIRefresh = now;
  
  switch (currentMode) {
    case MODE_MENU:
      menu.draw();
      break;
      
    case MODE_POMODORO:
      pomodoro.draw();
      break;
      
    case MODE_POMODORO_CONFIG:
      pomodoro.draw();
      break;
      
    case MODE_GYM:
      gym.draw();
      break;
      
    case MODE_STATS: {
      Statistics stats;
      storage.loadStats(stats);
      Screens::drawStats(stats);
      break;
    }
      
    case MODE_SETTINGS:
      Screens::drawSettings();
      break;
      
    case MODE_INFO:
      Screens::drawInfo();
      break;
  }
}

// ===== SETUP =====

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n\n");
  Serial.println("================================");
  Serial.println("   POMODORO TIMER v2.0");
  Serial.println("   ESP32 Modular System");
  Serial.println("================================");
  Serial.println();
  
  // Inicializar hardware
  Serial.println("Initializing hardware...");
  display.begin();
  input.begin();
  output.begin();
  sensors.begin();
  storage.begin();
  
  // Inicializar UI
  Serial.println("Initializing UI...");
  menu.begin();
  
  // Inicializar modos
  Serial.println("Initializing modes...");
  pomodoro.begin();
  gym.begin();
  
  // Pantalla de bienvenida
  display.clear();
  display.drawCentered("POMODORO", 40, COLOR_PRIMARY, 3);
  display.drawCentered("TIMER", 65, COLOR_PRIMARY, 3);
  display.drawCentered("v2.0", 95, COLOR_TEXT, 1);
  
  // Efectos de inicio
  output.setGreenLED(LED_BLINK_FAST);
  output.playSuccess();
  delay(2000);
  output.setGreenLED(LED_OFF);
  
  // Ir al menú principal
  display.clear();
  changeMode(MODE_MENU);
  
  Serial.println();
  Serial.println("✅ System ready!");
  Serial.println("================================");
  Serial.println();
}

// ===== LOOP PRINCIPAL =====

void loop() {
  unsigned long now = millis();
  
  // Actualizar inputs
  input.update();
  sensors.update();
  output.update();
  
  // Manejar modo actual
  switch (currentMode) {
    case MODE_MENU:
      handleMenuNavigation();
      break;
      
    case MODE_POMODORO:
      handlePomodoroMode();
      break;
      
    case MODE_POMODORO_CONFIG:
      handlePomodoroConfigMode();
      break;
      
    case MODE_GYM:
      handleGymMode();
      break;
      
    case MODE_STATS:
      handleStatsMode();
      break;
      
    case MODE_SETTINGS:
      handleSettingsMode();
      break;
      
    case MODE_INFO:
      handleInfoMode();
      break;
  }
  
  // Actualizar UI
  updateUI();
  
  // Pequeño delay para no saturar el CPU
  delay(10);
}