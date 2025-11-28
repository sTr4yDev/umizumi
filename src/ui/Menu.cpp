#include "Menu.h"

Menu menu;

void Menu::begin() {
  currentScreen = SCREEN_MAIN_MENU;
  previousScreen = SCREEN_MAIN_MENU;
  selectedOption = 0;
  lastPotValue = 0;
  needsRedraw = true;
  
  Serial.println("Menu system initialized (New structure)");
}

void Menu::update() {
  // Leer potenciómetro
  int potValue = input.getPotPosition();
  
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
      
    case SCREEN_PRODUCTIVITY_MENU:
      drawProductivityMenu();
      break;
      
    case SCREEN_GAMES_MENU:
      drawGamesMenu();
      break;
      
    case SCREEN_TOOLS_MENU:
      drawToolsMenu();
      break;
      
    case SCREEN_SETTINGS_MENU:
      drawSettingsMenu();
      break;
      
    case SCREEN_INFO_MENU:
      drawInfoMenu();
      break;
      
    default:
      break;
  }
}

// ===== DIBUJAR MENÚ PRINCIPAL =====

void Menu::drawMainMenu() {
  display.clear();
  drawHeader("MENU PRINCIPAL", COLOR_PRIMARY);
  
  const char* options[] = {
    "PRODUCTIVIDAD",
    "JUEGOS",
    "HERRAMIENTAS",
    "CONFIGURACION",
    "INFO"
  };
  
  for (int i = 0; i < MAIN_OPTION_COUNT; i++) {
    drawMenuOption(options[i], i, i == selectedOption);
  }
  
  // Footer
  display.drawText("POT: Navegar", 4, display.getHeight() - 10, COLOR_INFO, 1);
  display.drawRightAligned("SELECT: OK", display.getHeight() - 10, COLOR_INFO, 1);
}

// ===== DIBUJAR SUBMENÚ PRODUCTIVIDAD =====

void Menu::drawProductivityMenu() {
  display.clear();
  drawHeader("PRODUCTIVIDAD", COLOR_SUCCESS);
  
  const char* options[] = {
    "Pomodoro Timer",
    "Gym Mode",
    "Task Timer",
    "Estadisticas"
  };
  
  for (int i = 0; i < PROD_OPTION_COUNT; i++) {
    drawMenuOption(options[i], i, i == selectedOption);
  }
  
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

// ===== DIBUJAR SUBMENÚ JUEGOS =====

void Menu::drawGamesMenu() {
  display.clear();
  drawHeader("JUEGOS", COLOR_WARNING);
  
  const char* options[] = {
    "Snake",
    "Pong",
    "Tetris",
    "Flappy Bird",
    "Space Invaders"
  };
  
  for (int i = 0; i < GAME_OPTION_COUNT; i++) {
    drawMenuOption(options[i], i, i == selectedOption);
  }
  
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

// ===== DIBUJAR SUBMENÚ HERRAMIENTAS =====

void Menu::drawToolsMenu() {
  display.clear();
  drawHeader("HERRAMIENTAS", COLOR_SECONDARY);
  
  const char* options[] = {
    "Cronometro",
    "Cuenta Regresiva",
    "Calculadora",
    "Nivel",
    "Linterna"
  };
  
  for (int i = 0; i < TOOL_OPTION_COUNT; i++) {
    drawMenuOption(options[i], i, i == selectedOption);
  }
  
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

// ===== DIBUJAR SUBMENÚ CONFIGURACIÓN =====

void Menu::drawSettingsMenu() {
  display.clear();
  drawHeader("CONFIGURACION", COLOR_PRIMARY);
  
  const char* options[] = {
    "WiFi",
    "Display",
    "Sonido",
    "Sensores",
    "Reset"
  };
  
  for (int i = 0; i < SET_OPTION_COUNT; i++) {
    drawMenuOption(options[i], i, i == selectedOption);
  }
  
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

// ===== DIBUJAR SUBMENÚ INFO =====

void Menu::drawInfoMenu() {
  display.clear();
  drawHeader("INFORMACION", COLOR_INFO);
  
  const char* options[] = {
    "Sistema",
    "WiFi Status",
    "Stats Globales",
    "Acerca de"
  };
  
  for (int i = 0; i < INFO_OPTION_COUNT; i++) {
    drawMenuOption(options[i], i, i == selectedOption);
  }
  
  display.drawText("BACK: Menu", 4, display.getHeight() - 10, COLOR_INFO, 1);
}

// ===== UTILIDADES =====

void Menu::drawHeader(const char* title, uint16_t color) {
  display.fillRect(0, 0, display.getWidth(), 22, color);
  display.setTextSize(1);
  display.setTextColor(ST77XX_WHITE);
  
  int16_t x1, y1;
  uint16_t w, h;
  display.getTFT()->getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
  int x = (display.getWidth() - w) / 2;
  
  display.getTFT()->setCursor(x, 7);
  display.getTFT()->print(title);
  
  // Línea separadora
  display.drawLine(0, 22, display.getWidth(), 22, ST77XX_WHITE);
}

void Menu::drawMenuOption(const char* text, int index, bool selected, int yOffset) {
  int y = yOffset + (index * 16);
  
  if (selected) {
    // Fondo de selección
    display.fillRect(4, y - 2, display.getWidth() - 8, 14, COLOR_MENU_SELECT);
    
    // Cursor
    display.drawText(">", 8, y, COLOR_BG, 1);
    
    // Texto
    display.drawText(text, 20, y, COLOR_BG, 1);
  } else {
    // Texto normal
    display.drawText(text, 20, y, COLOR_MENU_TEXT, 1);
  }
}

// ===== NAVEGACIÓN =====

void Menu::goToScreen(MenuScreen screen) {
  previousScreen = currentScreen;
  currentScreen = screen;
  selectedOption = 0; // Reset selección al cambiar de menú
  needsRedraw = true;
  
  Serial.print("Screen changed: ");
  Serial.print(previousScreen);
  Serial.print(" -> ");
  Serial.println(currentScreen);
}

MenuScreen Menu::getCurrentScreen() {
  return currentScreen;
}

int Menu::getSelectedOption() {
  return selectedOption;
}

void Menu::handlePotChange(int value) {
  if (value < 0 || value > 100) return;
  
  int newOption = selectedOption;
  
  switch (currentScreen) {
    case SCREEN_MAIN_MENU:
      newOption = map(value, 0, 100, 0, MAIN_OPTION_COUNT - 1);
      break;
      
    case SCREEN_PRODUCTIVITY_MENU:
      newOption = map(value, 0, 100, 0, PROD_OPTION_COUNT - 1);
      break;
      
    case SCREEN_GAMES_MENU:
      newOption = map(value, 0, 100, 0, GAME_OPTION_COUNT - 1);
      break;
      
    case SCREEN_TOOLS_MENU:
      newOption = map(value, 0, 100, 0, TOOL_OPTION_COUNT - 1);
      break;
      
    case SCREEN_SETTINGS_MENU:
      newOption = map(value, 0, 100, 0, SET_OPTION_COUNT - 1);
      break;
      
    case SCREEN_INFO_MENU:
      newOption = map(value, 0, 100, 0, INFO_OPTION_COUNT - 1);
      break;
      
    default:
      return;
  }
  
  // Clamp
  if (newOption < 0) newOption = 0;
  
  if (newOption != selectedOption) {
    selectedOption = newOption;
    needsRedraw = true;
    output.playBeep();
  }
}

void Menu::handleSelectButton() {
  output.playBeep();
  output.setGreenLED(LED_BLINK_FAST);
  delay(100);
  output.setGreenLED(LED_OFF);
  
  // Desde Menú Principal
  if (currentScreen == SCREEN_MAIN_MENU) {
    switch (selectedOption) {
      case MAIN_PRODUCTIVITY:
        goToScreen(SCREEN_PRODUCTIVITY_MENU);
        break;
        
      case MAIN_GAMES:
        goToScreen(SCREEN_GAMES_MENU);
        break;
        
      case MAIN_TOOLS:
        goToScreen(SCREEN_TOOLS_MENU);
        break;
        
      case MAIN_SETTINGS:
        goToScreen(SCREEN_SETTINGS_MENU);
        break;
        
      case MAIN_INFO:
        goToScreen(SCREEN_INFO_MENU);
        break;
    }
    return;
  }
  
  // Desde Submenú Productividad
  if (currentScreen == SCREEN_PRODUCTIVITY_MENU) {
    switch (selectedOption) {
      case PROD_POMODORO:
        goToScreen(SCREEN_POMODORO_CONFIG);
        break;
        
      case PROD_GYM:
        goToScreen(SCREEN_GYM_RUNNING);
        break;
        
      case PROD_TASK_TIMER:
        goToScreen(SCREEN_TASK_TIMER);
        break;
        
      case PROD_STATS:
        goToScreen(SCREEN_STATS);
        break;
    }
    return;
  }
  
  // Desde Submenú Juegos
  if (currentScreen == SCREEN_GAMES_MENU) {
    switch (selectedOption) {
      case GAME_SNAKE:
        goToScreen(SCREEN_GAME_SNAKE);
        break;
        
      case GAME_PONG:
        goToScreen(SCREEN_GAME_PONG);
        break;
        
      case GAME_TETRIS:
        goToScreen(SCREEN_GAME_TETRIS);
        break;
        
      case GAME_FLAPPY:
        goToScreen(SCREEN_GAME_FLAPPY);
        break;
        
      case GAME_SPACE_INVADERS:
        goToScreen(SCREEN_GAME_SPACE_INVADERS);
        break;
    }
    return;
  }
  
  // (Continúa para otros submenús...)
}

void Menu::handleBackButton() {
  output.playBeep();
  
  // Si estamos en un submenú, volver al menú principal
  if (currentScreen != SCREEN_MAIN_MENU) {
    // Determinar si estamos en un submenú
    if (currentScreen >= SCREEN_PRODUCTIVITY_MENU && currentScreen <= SCREEN_STATS) {
      goToScreen(SCREEN_MAIN_MENU);
    } else if (currentScreen >= SCREEN_GAMES_MENU && currentScreen <= SCREEN_GAME_SPACE_INVADERS) {
      goToScreen(SCREEN_MAIN_MENU);
    } else if (currentScreen >= SCREEN_TOOLS_MENU && currentScreen <= SCREEN_TOOL_FLASHLIGHT) {
      goToScreen(SCREEN_MAIN_MENU);
    } else if (currentScreen >= SCREEN_SETTINGS_MENU && currentScreen <= SCREEN_SET_RESET) {
      goToScreen(SCREEN_MAIN_MENU);
    } else if (currentScreen >= SCREEN_INFO_MENU && currentScreen <= SCREEN_INFO_ABOUT) {
      goToScreen(SCREEN_MAIN_MENU);
    }
  }
}