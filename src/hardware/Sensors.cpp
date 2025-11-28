#include "Sensors.h"
#include <math.h>

Sensors sensors;

void Sensors::begin() {
  pinMode(PIN_SOUND, INPUT);
  
  soundState = false;
  soundLast = false;
  soundStartTime = 0;
  lastClapTime = 0;
  clapFlag = false;
  
  analogValue = 0;
  baselineValue = 0;
  peakValue = 0;
  smoothedValue = 0;
  
  Serial.println("╔════════════════════════════════════════════════╗");
  Serial.println("║  🎤 SENSOR DE SONIDO - MODO ANALÍTICO         ║");
  Serial.println("╚════════════════════════════════════════════════╝");
  Serial.println();
  
  // ⭐ CALIBRACIÓN AUTOMÁTICA
  Serial.println("📊 Iniciando calibración automática...");
  Serial.println("   Mantén silencio por 3 segundos...");
  Serial.println();
  
  int minVal = 4095;
  int maxVal = 0;
  long sumVal = 0;
  int samples = 0;
  
  // Medir durante 3 segundos
  unsigned long startTime = millis();
  while (millis() - startTime < 3000) {
    int reading = analogRead(PIN_SOUND);
    
    if (reading < minVal) minVal = reading;
    if (reading > maxVal) maxVal = reading;
    sumVal += reading;
    samples++;
    
    // Mostrar barra de progreso cada 300ms
    static unsigned long lastBar = 0;
    if (millis() - lastBar > 300) {
      Serial.print(".");
      lastBar = millis();
    }
    
    delay(10);
  }
  
  baselineValue = sumVal / samples;
  int noiseRange = maxVal - minVal;
  
  Serial.println(" ✓");
  Serial.println();
  Serial.println("═══════════════════════════════════════════════");
  Serial.println("📈 RESULTADOS DE CALIBRACIÓN:");
  Serial.println("───────────────────────────────────────────────");
  
  Serial.print("   Nivel base (promedio): ");
  Serial.print(baselineValue);
  Serial.print(" / 4095");
  Serial.print("  (");
  Serial.print(baselineValue * 100.0 / 4095.0, 1);
  Serial.println("%)");
  
  Serial.print("   Voltaje base:          ");
  Serial.print(baselineValue * 3.3 / 4095.0, 3);
  Serial.println(" V");
  
  Serial.print("   Rango mín-máx:         ");
  Serial.print(minVal);
  Serial.print(" - ");
  Serial.println(maxVal);
  
  Serial.print("   Variación de ruido:    ");
  Serial.print(noiseRange);
  Serial.println(" unidades");
  
  // Calcular umbral dinámico
  int threshold = baselineValue + (noiseRange * 5);
  Serial.println();
  Serial.print("🎯 Umbral de detección:    ");
  Serial.print(threshold);
  Serial.print(" (base + ");
  Serial.print(noiseRange * 5);
  Serial.println(")");
  
  Serial.println("═══════════════════════════════════════════════");
  Serial.println();
  Serial.println("✅ Sistema listo para detectar CLAPS");
  Serial.println("👏 Haz un CLAP para ver el análisis completo");
  Serial.println();
  Serial.println("═══════════════════════════════════════════════");
  Serial.println();
  
  // Configurar valor inicial suavizado
  smoothedValue = baselineValue;
}

void Sensors::update() {
  // ⭐ Leer valor analógico
  analogValue = analogRead(PIN_SOUND);
  
  // ⭐ Suavizado exponencial (alpha = 0.3)
  smoothedValue = (smoothedValue * 0.7) + (analogValue * 0.3);
  
  // Actualizar pico si es mayor
  if (analogValue > peakValue) {
    peakValue = analogValue;
  }
  
  // Calcular diferencia respecto al baseline
  int delta = analogValue - baselineValue;
  
  // Aproximación de decibeles (fórmula simplificada)
  // dB ≈ 20 * log10(V / V_ref)
  // Usando baseline como referencia = 0dB
  float approxDB = 0;
  if (delta > 0) {
    approxDB = 20.0 * log10((float)analogValue / (float)baselineValue);
  }
  
  // ⭐ VISUALIZACIÓN EN TIEMPO REAL (cada 150ms)
  static unsigned long lastDisplay = 0;
  if (millis() - lastDisplay > 150) {
    // Crear barra visual
    int barLength = map(analogValue, 0, 4095, 0, 50);
    if (barLength < 0) barLength = 0;
    if (barLength > 50) barLength = 50;
    
    // Color de la barra según intensidad
    char barChar = '─';
    if (delta > baselineValue * 0.5) barChar = '█';      // Muy alto
    else if (delta > baselineValue * 0.2) barChar = '▓'; // Alto
    else if (delta > baselineValue * 0.1) barChar = '▒'; // Medio
    else if (delta > 50) barChar = '░';                   // Bajo
    
    Serial.print(analogValue);
    Serial.print("\t");
    Serial.print(analogValue * 3.3 / 4095.0, 2);
    Serial.print("V\t");
    Serial.print("Δ");
    Serial.print(delta);
    Serial.print("\t");
    
    if (approxDB > 0) {
      Serial.print(approxDB, 1);
      Serial.print("dB");
    } else {
      Serial.print("---dB");
    }
    
    Serial.print("\t|");
    for (int i = 0; i < barLength; i++) {
      Serial.print(barChar);
    }
    Serial.println();
    
    lastDisplay = millis();
  }
  
  // ⭐ DETECCIÓN DIGITAL
  int digitalValue = digitalRead(PIN_SOUND);
  soundState = (digitalValue == LOW);
  
  // Mostrar eventos de cambio digital
  static bool lastDigitalState = false;
  if (soundState != lastDigitalState) {
    Serial.println();
    Serial.println("╔════════════════════════════════════════════════╗");
    
    if (soundState) {
      Serial.println("║  🔊 SEÑAL DIGITAL ACTIVADA (LOW)              ║");
    } else {
      Serial.println("║  🔇 SEÑAL DIGITAL DESACTIVADA (HIGH)          ║");
    }
    
    Serial.println("╠════════════════════════════════════════════════╣");
    Serial.print("║  Valor analógico: ");
    Serial.print(analogValue);
    Serial.print(" (");
    Serial.print(analogValue * 3.3 / 4095.0, 2);
    Serial.println("V)");
    
    Serial.print("║  Delta vs base:   ");
    Serial.print(delta);
    Serial.println(" unidades");
    
    if (approxDB > 0) {
      Serial.print("║  Nivel aprox:     ~");
      Serial.print(approxDB, 1);
      Serial.println(" dB");
    }
    
    Serial.println("╚════════════════════════════════════════════════╝");
    Serial.println();
    
    lastDigitalState = soundState;
  }
  
  // Detectar inicio de sonido
  if (soundState && !soundLast) {
    soundStartTime = millis();
    peakValue = analogValue; // Resetear pico
    
    Serial.println("🎤 ▶ INICIO DE EVENTO DE SONIDO");
  }
  
  // Detectar fin de sonido
  if (!soundState && soundLast) {
    unsigned long duration = millis() - soundStartTime;
    unsigned long timeSinceLastClap = millis() - lastClapTime;
    
    // Calcular dB del pico
    float peakDB = 0;
    if (peakValue > baselineValue) {
      peakDB = 20.0 * log10((float)peakValue / (float)baselineValue);
    }
    
    Serial.println();
    Serial.println("═══════════════════════════════════════════════");
    Serial.println("🎤 ⏹ FIN DE EVENTO DE SONIDO");
    Serial.println("───────────────────────────────────────────────");
    Serial.print("   Duración:         ");
    Serial.print(duration);
    Serial.println(" ms");
    
    Serial.print("   Pico analógico:   ");
    Serial.print(peakValue);
    Serial.print(" (");
    Serial.print(peakValue * 3.3 / 4095.0, 2);
    Serial.println("V)");
    
    Serial.print("   Pico estimado:    ~");
    Serial.print(peakDB, 1);
    Serial.println(" dB sobre base");
    
    Serial.print("   Tiempo desde último: ");
    Serial.print(timeSinceLastClap);
    Serial.println(" ms");
    
    // Verificar si es un CLAP válido
    bool validDuration = (duration >= CLAP_DURATION_MIN && duration <= CLAP_DURATION_MAX);
    bool validCooldown = (timeSinceLastClap >= CLAP_COOLDOWN_MS);
    
    Serial.println("───────────────────────────────────────────────");
    Serial.print("   Duración válida:  ");
    Serial.println(validDuration ? "✓ SÍ" : "✗ NO");
    
    Serial.print("   Cooldown OK:      ");
    Serial.println(validCooldown ? "✓ SÍ" : "✗ NO");
    
    if (validDuration && validCooldown) {
      clapFlag = true;
      lastClapTime = millis();
      
      Serial.println("═══════════════════════════════════════════════");
      Serial.println();
      Serial.println("╔════════════════════════════════════════════════╗");
      Serial.println("║                                                ║");
      Serial.println("║        ✅ 👏 CLAP VÁLIDO DETECTADO 👏 ✅       ║");
      Serial.println("║                                                ║");
      Serial.println("╚════════════════════════════════════════════════╝");
      Serial.println();
      Serial.println("═══════════════════════════════════════════════");
    } else {
      Serial.println("═══════════════════════════════════════════════");
      Serial.println("   ❌ EVENTO RECHAZADO");
      
      if (!validDuration) {
        Serial.print("      • Duración fuera de rango (");
        Serial.print(CLAP_DURATION_MIN);
        Serial.print("-");
        Serial.print(CLAP_DURATION_MAX);
        Serial.println("ms)");
      }
      
      if (!validCooldown) {
        Serial.print("      • Cooldown no cumplido (min: ");
        Serial.print(CLAP_COOLDOWN_MS);
        Serial.println("ms)");
      }
      
      Serial.println("═══════════════════════════════════════════════");
    }
    Serial.println();
  }
  
  soundLast = soundState;
}

bool Sensors::clapDetected() {
  return clapFlag;
}

void Sensors::resetClap() {
  clapFlag = false;
}

// ⭐ NUEVOS MÉTODOS
int Sensors::getAnalogValue() {
  return analogValue;
}

float Sensors::getVoltage() {
  return analogValue * 3.3 / 4095.0;
}

float Sensors::getApproxDB() {
  if (analogValue > baselineValue) {
    return 20.0 * log10((float)analogValue / (float)baselineValue);
  }
  return 0;
}

int Sensors::getBaselineValue() {
  return baselineValue;
}