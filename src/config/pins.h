#ifndef PINS_H
#define PINS_H

// ===== PINES DEL DISPLAY TFT ST7735 =====
#define PIN_TFT_CS      5     // Chip Select
#define PIN_TFT_DC      2     // Data/Command
#define PIN_TFT_RST     4     // Reset

// ===== PINES DE ENTRADA =====
#define PIN_POT         34    // Potenciómetro ADC
#define PIN_BTN_SELECT  14    // Botón SELECT
#define PIN_BTN_BACK    12    // Botón BACK

// ===== PINES DE SALIDA =====
#define PIN_LED_GREEN   32    // LED Verde
#define PIN_LED_RED     33    // LED Rojo
#define PIN_BUZZER      25    // Buzzer TMB12A05

// ===== SENSOR DE SONIDO =====
#define PIN_SOUND       26    // Sensor de sonido KY-037

// ===== ENUMS =====
enum LEDState {
  LED_OFF,
  LED_ON,
  LED_BLINK_SLOW,
  LED_BLINK_FAST
};

#endif