#ifndef PINS_H
#define PINS_H

// ===== DISPLAY TFT ST7735 =====
#define PIN_TFT_CS      5
#define PIN_TFT_DC      2 //A0
#define PIN_TFT_RST     4
// SPI hardware: MOSI=23, SCK=18

// ===== INPUTS =====
#define PIN_POT         34    // Potenciómetro ADC (también botón)
#define PIN_BTN_SELECT  14    // Botón SELECT
#define PIN_BTN_BACK    12    // Botón BACK

// ===== OUTPUTS =====
#define PIN_LED_GREEN   32    // LED Verde (confirmación)
#define PIN_LED_RED     33    // LED Rojo (alerta)
#define PIN_BUZZER      25    // Buzzer pasivo

// ===== SENSORS =====
#define PIN_SOUND       26    // Sensor de sonido (CLAP)

#endif