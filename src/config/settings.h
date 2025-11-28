#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

// ===== TIEMPOS POR DEFECTO (segundos) =====
#define DEFAULT_POMODORO_TIME    1500   // 25 minutos
#define DEFAULT_SHORT_BREAK      300    // 5 minutos
#define DEFAULT_LONG_BREAK       900    // 15 minutos
#define POMODOROS_UNTIL_LONG     4      // Cada 4 pomodoros

// ===== GYM =====
#define MIN_SERIE_DURATION       800    // 0.8 segundos mínimo

// ===== UI =====
#define UI_REFRESH_MS            300    // Actualizar pantalla cada 300ms
#define MENU_DEBOUNCE_MS         200    // Debounce navegación menú

// ===== POTENCIÓMETRO =====
#define POT_THRESHOLD            100    // Cambio mínimo para detectar movimiento
#define POT_SAMPLES              5      // Muestras para promediar

// ===== CLAP DETECTION =====
#define CLAP_COOLDOWN_MS         500    // Tiempo entre claps
#define CLAP_DURATION_MIN        50     // Duración mínima sonido
#define CLAP_DURATION_MAX        300    // Duración máxima sonido

// ===== BUZZER =====
#define BUZZER_FREQ_BEEP         2000   // 2kHz para beep
#define BUZZER_FREQ_MED          2300   // 2.3kHz para TMB12A05 (frecuencia resonante)
#define BUZZER_FREQ_ALERT        1000   // 1kHz para alerta
#define BUZZER_DURATION          200    // Duración beep (ms)

// ===== LEDS =====
#define LED_BLINK_FAST_MS        100    // Parpadeo rápido (ms)
#define LED_BLINK_SLOW_MS        500    // Parpadeo lento (ms)

#endif