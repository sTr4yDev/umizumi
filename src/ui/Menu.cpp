#include "Menu.h"

Menu menu;

void Menu::begin() {
  currentScreen = SCREEN_MAIN_MENU;
  previousScreen = SCREEN_MAIN_MENU;
  selectedOption = 0;
  lastPotValue = 0;
  needsRedraw = true;
  
  // ✅ Inicializar sistema de notificación
  screenChanged = false;
  pendingScreen = SCREEN_MAIN_MENU;
  
  Serial.println("Menu system initialized");
}

void Menu::update() {
  // Leer potenciómetro
  int potValue = input.getPotPosition(); // 0-100
  
  // Detectar cambio significativo
  if (abs(potValue - lastPotValue) > 5) {
    handlePotChange(potValue);
    lastPotValue = potValue;
  }
  
  // Leer botones
  ButtonEvent event = input.getButtonEvent();
  
  switch (event) {
    case BTN_SELECT_RELEASED:
      handleSelectButton();
      break;
      
    case BTN_BACK_RELEASED:
      handleBackButton();
      break;
      
    default:
      break;
  }
  
  // Redibujar si es necesario
  if (needsRedraw) {
    draw();
    needsRedraw = false;
  }
}

void Menu::draw() {
  switch (currentScreen) {
    case SCREEN_MAIN_MENU:
      drawMainMenu();
      break;
      
    // Otros screens se manejarán en Screens.cpp
    default:
      break;
  }
}

void Menu::drawMainMenu() {
  display.clear();
  
  // Título
  display.drawCentered("MENU PRINCIPAL", 8, COLOR_PRIMARY, 2);
  
  // Línea separadora
  display.drawLine(10, 28, display.getWidth() - 10, 28, COLOR_PRIMARY);
  
  // Opciones
  const char* options[] = {
    "Pomodoro",
    "Gym Mode",
    "Config",
    "Stats",
    "Info"
  };
  
  int startY = 40;
  int spacing = 16;
  
  for (int i = 0; i < MENU_OPTION_COUNT; i++) {
    drawMenuOption(options[i], i, i == selectedOption);
  }
  
  // Indicador de navegación
  display.drawText("POT: Navegar", 4, display.getHeight() - 10, COLOR_INFO, 1);
  display.drawRightAligned("SELECT: OK", display.getHeight() - 10, COLOR_INFO, 1);
}

void Menu::drawMenuOption(const char* text, int index, bool selected) {
  int y = 40 + (index * 16);
  
  if (selected) {
    // Fondo de selección
    display.fillRect(8, y - 2, display.getWidth() - 16, 14, COLOR_MENU_SELECT);
    
    // Cursor
    display.drawText(">", 12, y, COLOR_BG, 1);
    
    // Texto
    display.drawText(text, 24, y, COLOR_BG, 1);
  } else {
    // Texto normal
    display.drawText(text, 24, y, COLOR_MENU_TEXT, 1);
  }
}

void Menu::goToScreen(MenuScreen screen) {
  previousScreen = currentScreen;
  currentScreen = screen;
  needsRedraw = true;
  
  // ✅ Notificar cambio de pantalla
  screenChanged = true;
  pendingScreen = screen;
  
  Serial.print("Menu: Screen changed to ");
  Serial.println(screen);
}

MenuScreen Menu::getCurrentScreen() {
  return currentScreen;
}

int Menu::getSelectedOption() {
  return selectedOption;
}

// ✅ NUEVO: Sistema de notificación
bool Menu::hasScreenChanged() {
  return screenChanged;
}

MenuScreen Menu::getPendingScreen() {
  return pendingScreen;
}

void Menu::clearScreenChange() {
  screenChanged = false;
}

void Menu::handlePotChange(int value) {
  // Mapear 0-100 a opciones del menú
  if (currentScreen == SCREEN_MAIN_MENU) {
    int newOption = map(value, 0, 100, 0, MENU_OPTION_COUNT - 1);
    
    if (newOption != selectedOption) {
      selectedOption = newOption;
      needsRedraw = true;
      output.playBeep();
      
      Serial.print("Option selected: ");
      Serial.println(selectedOption);
    }
  }
}

void Menu::handleSelectButton() {
  output.playBeep();
  output.setGreenLED(LED_BLINK_FAST);
  delay(100);
  output.setGreenLED(LED_OFF);
  
  Serial.print("Menu: SELECT pressed on option ");
  Serial.println(selectedOption);
  
  if (currentScreen == SCREEN_MAIN_MENU) {
    switch (selectedOption) {
      case MENU_POMODORO:
        goToScreen(SCREEN_POMODORO_CONFIG);
        break;
        
      case MENU_GYM:
        goToScreen(SCREEN_GYM_RUNNING);
        break;
        
      case MENU_SETTINGS:
        goToScreen(SCREEN_SETTINGS);
        break;
        
      case MENU_STATS:
        goToScreen(SCREEN_STATS);
        break;
        
      case MENU_INFO:
        goToScreen(SCREEN_INFO);
        break;
    }
  }
}

void Menu::handleBackButton() {
  output.playBeep();
  
  // Regresar al menú principal desde cualquier pantalla
  if (currentScreen != SCREEN_MAIN_MENU) {
    goToScreen(SCREEN_MAIN_MENU);
  }
}