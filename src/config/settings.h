#ifndef SETTINGS_H
#define SETTINGS_H

// ===== TIMING CONFIGURATION =====
#define UI_REFRESH_MS 300        // Tiempo de refresco de la interfaz
#define DEBOUNCE_MS 50           // Debounce para botones
#define LONG_PRESS_MS 1000       // Tiempo para press largo
#define POT_SAMPLES 5            // Muestras para promedio del potenciómetro

// ===== POMODORO DEFAULTS =====
#define DEFAULT_POMODORO_TIME 1500    // 25 minutos en segundos
#define DEFAULT_SHORT_BREAK 300       // 5 minutos en segundos  
#define DEFAULT_LONG_BREAK 900        // 15 minutos en segundos
#define POMODOROS_UNTIL_LONG 4        // 4 pomodoros antes de break largo

// ===== GYM DEFAULTS =====
#define DEFAULT_GYM_SERIE_DURATION 1000  // 1 segundo en milisegundos
#define DEFAULT_GYM_TARGET_SERIES 10     // 10 series por defecto

// ===== TASK TIMER DEFAULTS =====
#define DEFAULT_TASK_TIME 1800          // 30 minutos en segundos

// ===== SENSOR THRESHOLDS =====
#define SOUND_THRESHOLD 2000            // Umbral para detección de sonido
#define SOUND_COOLDOWN_MS 500           // Tiempo de espera entre detecciones

// ===== DISPLAY CONFIGURATION =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#define ROTATION 3

// ===== COLOR DEFINITIONS =====
// Colores principales para la interfaz
#define COLOR_BG ST77XX_BLACK
#define COLOR_TEXT ST77XX_WHITE
#define COLOR_PRIMARY ST77XX_BLUE
#define COLOR_SUCCESS ST77XX_GREEN
#define COLOR_WARNING ST77XX_YELLOW
#define COLOR_DANGER ST77XX_RED
#define COLOR_INFO ST77XX_CYAN
#define COLOR_SECONDARY ST77XX_MAGENTA

// Estados específicos
#define COLOR_RUNNING ST77XX_GREEN
#define COLOR_PAUSED ST77XX_YELLOW
#define COLOR_BREAK ST77XX_CYAN
#define COLOR_IDLE ST77XX_WHITE
#define COLOR_MENU_SELECT ST77XX_BLUE

// ===== HARDWARE PINS =====
#define PIN_BUZZER 25
#define PIN_LED 26
#define PIN_SOUND_SENSOR 34
#define PIN_BUTTON_1 32
#define PIN_BUTTON_2 33
#define PIN_POT A0

// ===== AUDIO CONFIG =====
#define BUZZER_FREQ 2000
#define BUZZER_DURATION 100

// ===== LED PATTERNS =====
enum LEDState {
  LED_OFF,
  LED_ON,
  LED_BLINK_SLOW,    // 1000ms interval
  LED_BLINK_FAST,    // 250ms interval
  LED_BLINK_RAPID    // 100ms interval (para notificaciones urgentes)
};

// ===== POMODORO STATES =====
enum PomodoroState {
  POMODORO_WORK,
  POMODORO_SHORT_BREAK,
  POMODORO_LONG_BREAK,
  POMODORO_PAUSED,
  POMODORO_FINISHED
};

// ===== SYSTEM MODES =====
enum SystemMode {
  MODE_MENU,
  MODE_POMODORO,
  MODE_GYM,
  MODE_TASK_TIMER,
  MODE_STATS,
  MODE_SETTINGS,
  MODE_INFO
};

// ===== MENU CONFIGURATION =====
#define MENU_ITEMS 6
const char* MENU_OPTIONS[MENU_ITEMS] = {
  "Pomodoro Timer",
  "Gym Mode", 
  "Task Timer",
  "Estadisticas",
  "Configuracion",
  "Info Sistema"
};

#endif