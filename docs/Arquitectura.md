# Arquitectura del Proyecto Umizumi

## Descripción General

Umizumi es un temporizador Pomodoro basado en ESP32 con pantalla TFT, que también incluye un modo Gym con detección de palmadas (CLAP). El proyecto está estructurado de manera modular para facilitar el mantenimiento y la extensión.

## Plataforma

- **Microcontrolador**: ESP32
- **Framework**: Arduino
- **Build System**: PlatformIO
- **Display**: ST7735/ST7789 TFT 128x160
- **Puerto de comunicación**: COM5 (configurable en platformio.ini)

## Estructura de Directorios

```
umizumi/
├── config/              # Configuración global del sistema
│   ├── pins.h          # Definición de pines del ESP32
│   ├── settings.h      # Constantes de tiempo, umbrales y configuraciones
│   └── colors.h        # Paleta de colores para la UI
│
├── hardware/           # Abstracción de hardware
│   ├── Display.h/cpp   # Control de pantalla TFT (ST7735)
│   ├── Input.h/cpp     # Manejo de botones y potenciómetro
│   ├── Output.h/cpp    # Control de LEDs y buzzer
│   ├── Sensors.h/cpp   # Sensor de sonido (CLAP detection)
│   └── Storage.h/cpp   # Almacenamiento persistente (SPIFFS/Preferences)
│
├── ui/                 # Interfaz de usuario
│   ├── Menu.h/cpp      # Sistema de menú principal
│   └── Screens.h/cpp   # Pantallas estáticas (Stats, Settings, Info)
│
├── modes/              # Modos de operación
│   ├── Pomodoro.h/cpp  # Lógica del temporizador Pomodoro
│   └── Gym.h/cpp       # Modo Gym con contador de series
│
├── src/
│   └── main.cpp        # Loop principal y coordinación de modos
│
├── docs/               # Documentación
├── platformio.ini      # Configuración del proyecto PlatformIO
└── README.md           # Documentación general
```

## Módulos Principales

### 1. Config (Configuración)

#### pins.h
Define todos los pines del ESP32 utilizados por el proyecto:

**Display TFT:**
- `PIN_TFT_CS`: 5 (Chip Select)
- `PIN_TFT_DC`: 2 (Data/Command)
- `PIN_TFT_RST`: 4 (Reset)
- SPI hardware: MOSI=23, SCK=18

**Inputs:**
- `PIN_POT`: 34 (Potenciómetro ADC)
- `PIN_BTN_SELECT`: 14
- `PIN_BTN_BACK`: 12

**Outputs:**
- `PIN_LED_GREEN`: 32
- `PIN_LED_RED`: 33
- `PIN_BUZZER`: 25

**Sensors:**
- `PIN_SOUND`: 26 (Sensor de sonido para CLAP)

#### settings.h
Contiene todas las constantes configurables:

**Tiempos por defecto (en segundos):**
- `DEFAULT_POMODORO_TIME`: 1500 (25 minutos)
- `DEFAULT_SHORT_BREAK`: 300 (5 minutos)
- `DEFAULT_LONG_BREAK`: 900 (15 minutos)
- `POMODOROS_UNTIL_LONG`: 4

**Gym:**
- `MIN_SERIE_DURATION`: 800ms

**UI:**
- `UI_REFRESH_MS`: 300ms
- `MENU_DEBOUNCE_MS`: 200ms

**Potenciómetro:**
- `POT_THRESHOLD`: 100 (cambio mínimo para detectar movimiento)
- `POT_SAMPLES`: 5 (muestras para promediar)

**CLAP Detection:**
- `CLAP_COOLDOWN_MS`: 500ms
- `CLAP_DURATION_MIN`: 50ms
- `CLAP_DURATION_MAX`: 300ms

**Buzzer (frecuencias en Hz):**
- `BUZZER_FREQ_BEEP`: 2000
- `BUZZER_FREQ_MED`: 2300 (frecuencia resonante TMB12A05)
- `BUZZER_FREQ_ALERT`: 1000
- `BUZZER_DURATION`: 200ms

**LEDs (tiempos en ms):**
- `LED_BLINK_FAST_MS`: 100
- `LED_BLINK_SLOW_MS`: 500

**IMPORTANTE**: Las constantes de tiempo LED terminan en `_MS` para evitar conflictos con el enum `LEDState`.

#### colors.h
Define la paleta de colores para el UI usando formato RGB565.

### 2. Hardware (Abstracción de Hardware)

#### Display.h/cpp
Abstracción de la pantalla TFT.

**Constructor:**
- `Display()`: Constructor que inicializa el objeto Adafruit_ST7735 mediante lista de inicialización con los pines CS, DC y RST

**Funciones principales:**
- `begin()`: Inicializa la pantalla (initR, fillScreen, setRotation, setTextWrap)
- `clear()`: Limpia la pantalla con el color de fondo
- `setRotation(uint8_t)`: Configura la rotación de la pantalla
- `drawText(text, x, y, color, size)`: Dibuja texto en posición específica
- `drawCentered(text, y, color, size)`: Dibuja texto centrado horizontalmente
- `drawRightAligned(text, y, color, size)`: Dibuja texto alineado a la derecha
- `drawRect()`, `fillRect()`: Dibujar rectángulos
- `drawCircle()`, `fillCircle()`: Dibujar círculos
- `drawLine()`: Dibuja líneas
- `fillScreen(color)`: Rellena toda la pantalla con un color
- `getWidth()`, `getHeight()`: Obtiene dimensiones de la pantalla
- `getTFT()`: Retorna un puntero al objeto Adafruit_ST7735 para acceso directo

**Miembro privado:**
- `Adafruit_ST7735 tft`: Objeto de la librería Adafruit inicializado en el constructor

**Instancia global**: `display`

#### Input.h/cpp
Manejo de entradas (botones y potenciómetro).

**Enums importantes:**
```cpp
enum ButtonEvent {
  BTN_NONE,
  BTN_SELECT_PRESSED,
  BTN_SELECT_RELEASED,
  BTN_SELECT_LONG_PRESS,
  BTN_BACK_PRESSED,
  BTN_BACK_RELEASED,
  BTN_BACK_LONG_PRESS
};
```

**Funciones principales:**
- `begin()`: Inicializa los pines de entrada
- `update()`: Actualiza el estado de los inputs (llamar en loop)
- `getPotValue()`: Obtiene el valor raw del potenciómetro (0-4095)
- `getPotPosition()`: Obtiene la posición normalizada del potenciómetro (0-100)
- `potChanged()`: Verifica si el potenciómetro cambió significativamente
- `getButtonEvent()`: Obtiene el evento de botón actual
- `isSelectPressed()`: Verifica si el botón Select está presionado
- `isBackPressed()`: Verifica si el botón Back está presionado

**Instancia global**: `input`

#### Output.h/cpp
Control de LEDs y buzzer.

**Enums importantes:**
```cpp
enum LEDState {
  LED_OFF,
  LED_ON,
  LED_BLINK_FAST,  // Parpadeo rápido (100ms)
  LED_BLINK_SLOW   // Parpadeo lento (500ms)
};
```

**Funciones principales:**
- `begin()`: Inicializa LEDs y buzzer con PWM
- `update()`: Actualiza el estado de los LEDs (llamar en loop)
- `setGreenLED(LEDState)`: Establece el estado del LED verde
- `setRedLED(LEDState)`: Establece el estado del LED rojo
- `greenOn()`, `greenOff()`: Control directo del LED verde
- `redOn()`, `redOff()`: Control directo del LED rojo
- `allLedsOff()`: Apaga todos los LEDs
- `playBeep()`: Beep simple
- `playError()`: Sonido de error
- `playSuccess()`: Sonido de éxito
- `playPomodoroComplete()`: Melodía de pomodoro completado
- `playBreakTime()`: Melodía de break
- `stopBuzzer()`: Detiene el buzzer

**Método privado:**
- `updateLED(pin, state, ledOn, lastBlink)`: Actualiza el estado de un LED individual

**Implementación del buzzer:**
- Usa PWM (canal 0) con `ledcSetup()`, `ledcAttachPin()`, `ledcWriteTone()`
- Frecuencia resonante del TMB12A05: 2300 Hz (`BUZZER_FREQ_MED`)

**Instancia global**: `output`

#### Sensors.h/cpp
Manejo de sensores (actualmente solo sensor de sonido).

**Funciones principales:**
- `begin()`: Inicializa sensores
- `update()`: Actualiza el estado (llamar en loop)
- `clapDetected()`: Verifica si se detectó un CLAP
- `resetClap()`: Resetea el estado del CLAP

**Instancia global**: `sensors`

#### Storage.h/cpp
Almacenamiento persistente usando Preferences (ESP32 NVS).

**Estructuras:**
```cpp
struct PomodoroConfig {
  uint16_t workTime;      // Segundos
  uint16_t shortBreak;    // Segundos
  uint16_t longBreak;     // Segundos
  uint8_t cyclesUntilLong;
};

struct GymConfig {
  uint16_t minSerieDuration; // Milisegundos
  uint16_t targetSeries;
};

struct Statistics {
  uint32_t pomodorosCompleted;
  uint32_t gymSeriesCompleted;
  uint32_t soundInterruptions;
  uint32_t totalWorkTime;      // Segundos totales trabajados
};
```

**Funciones principales:**
- `begin()`: Inicializa el sistema de almacenamiento
- `loadPomodoroConfig(PomodoroConfig&)`, `savePomodoroConfig(const PomodoroConfig&)`: Config del Pomodoro
- `loadGymConfig(GymConfig&)`, `saveGymConfig(const GymConfig&)`: Config del Gym
- `loadStats(Statistics&)`, `saveStats(const Statistics&)`: Estadísticas
- `incrementPomodoros()`: Incrementa contador de pomodoros completados
- `incrementGymSeries()`: Incrementa contador de series de gym
- `incrementInterruptions()`: Incrementa contador de interrupciones de sonido
- `addWorkTime(uint32_t)`: Añade tiempo trabajado en segundos
- `resetStats()`: Reinicia todas las estadísticas

**Miembro privado:**
- `Preferences prefs`: Objeto de la librería Preferences de ESP32

**Instancia global**: `storage`

### 3. UI (Interfaz de Usuario)

#### Menu.h/cpp
Sistema de menú con navegación por potenciómetro.

**Enums importantes:**
```cpp
enum MenuScreen {
  SCREEN_MAIN_MENU,
  SCREEN_POMODORO_RUNNING,
  SCREEN_POMODORO_CONFIG,
  SCREEN_GYM_RUNNING,
  SCREEN_GYM_CONFIG,
  SCREEN_SETTINGS,
  SCREEN_STATS,
  SCREEN_INFO
};

enum MainMenuOption {
  MENU_POMODORO = 0,
  MENU_GYM = 1,
  MENU_SETTINGS = 2,
  MENU_STATS = 3,
  MENU_INFO = 4,
  MENU_OPTION_COUNT = 5
};
```

**Funciones principales:**
- `begin()`: Inicializa el menú
- `update()`: Actualiza navegación (llamar en loop)
- `draw()`: Dibuja el menú actual
- `goToScreen(MenuScreen)`: Cambia de pantalla
- `getCurrentScreen()`: Obtiene la pantalla actual
- `getSelectedOption()`: Obtiene la opción seleccionada actualmente
- `handlePotChange(int)`: Maneja cambios del potenciómetro
- `handleSelectButton()`: Maneja presión del botón Select
- `handleBackButton()`: Maneja presión del botón Back

**Métodos privados:**
- `drawMainMenu()`: Dibuja el menú principal
- `drawMenuOption(text, index, selected)`: Dibuja una opción de menú

**Instancia global**: `menu`

#### Screens.h/cpp
Pantallas estáticas y helpers de UI. Todos los métodos son estáticos.

**Funciones estáticas:**
- `drawPomodoroConfig(PomodoroConfig&, int selectedField)`: Dibuja pantalla de configuración del Pomodoro
- `drawPomodoroRunning(const char* state, unsigned long timeLeft, uint32_t pomodorosCount)`: Dibuja pantalla del Pomodoro en ejecución
- `drawGymRunning(unsigned long elapsed, uint32_t seriesCount)`: Dibuja pantalla del Gym en ejecución
- `drawStats(const Statistics&)`: Dibuja pantalla de estadísticas
- `drawSettings()`: Dibuja pantalla de configuración general
- `drawInfo()`: Dibuja pantalla de información del sistema
- `drawProgressBar(int x, int y, int width, int height, int percent, uint16_t color)`: Helper para dibujar barras de progreso
- `drawTimeString(char* buffer, unsigned long seconds)`: Helper para formatear tiempo en formato MM:SS

### 4. Modes (Modos de Operación)

#### Pomodoro.h/cpp
Implementación completa del temporizador Pomodoro.

**Estados:**
```cpp
enum PomodoroState {
  POM_IDLE,
  POM_RUNNING,
  POM_PAUSED,
  POM_SHORT_BREAK,
  POM_LONG_BREAK,
  POM_BREAK_PAUSED
};
```

**Funciones públicas:**
- `begin()`: Inicializa el modo
- `update()`: Actualiza la lógica (llamar en loop)
- `draw()`: Dibuja la pantalla del Pomodoro
- `start()`: Inicia un nuevo pomodoro
- `pause()`: Pausa el pomodoro actual
- `resume()`: Reanuda el pomodoro pausado
- `stop()`: Detiene el pomodoro completamente
- `skipBreak()`: Salta el break actual
- `getState()`: Obtiene el estado actual
- `getStateString()`: Obtiene el nombre del estado como string
- `getTimeLeft()`: Obtiene el tiempo restante en segundos
- `getPomodorosCompleted()`: Obtiene el número de pomodoros completados
- `enterConfigMode()`: Entra en modo configuración
- `exitConfigMode()`: Sale del modo configuración
- `isInConfigMode()`: Verifica si está en modo configuración
- `adjustConfigValue(int)`: Ajusta valores con potenciómetro
- `nextConfigField()`: Avanza al siguiente campo de configuración
- `saveConfiguration()`: Guarda la configuración actual

**Métodos privados:**
- `transitionTo(PomodoroState)`: Transiciona a un nuevo estado
- `finishPomodoro()`: Finaliza un pomodoro
- `finishBreak()`: Finaliza un break
- `playStateSound()`: Reproduce el sonido correspondiente al estado

**Instancia global**: `pomodoro`

#### Gym.h/cpp
Modo Gym con contador de series y detección de CLAP.

**Estados:**
```cpp
enum GymState {
  GYM_IDLE,
  GYM_RUNNING,
  GYM_PAUSED
};
```

**Funciones públicas:**
- `begin()`: Inicializa el modo
- `update()`: Actualiza la lógica (llamar en loop)
- `draw()`: Dibuja la pantalla del Gym
- `handleClap()`: Procesa detección de CLAP para contar series
- `start()`: Inicia sesión de gym
- `pause()`: Pausa la sesión actual
- `resume()`: Reanuda la sesión pausada
- `stop()`: Detiene la sesión completamente
- `reset()`: Reinicia el contador de series
- `getState()`: Obtiene el estado actual
- `getElapsedTime()`: Obtiene el tiempo transcurrido en milisegundos
- `getSeriesCount()`: Obtiene el número de series realizadas

**Métodos privados:**
- `addSerie()`: Incrementa el contador de series

**Instancia global**: `gym`

### 5. Main (src/main.cpp)

Coordina todos los módulos y maneja la máquina de estados de la aplicación.

**Modos de aplicación:**
```cpp
enum AppMode {
  MODE_MENU,
  MODE_POMODORO,
  MODE_POMODORO_CONFIG,
  MODE_GYM,
  MODE_SETTINGS,
  MODE_STATS,
  MODE_INFO
};
```

**Funciones principales:**
- `setup()`: Inicializa todos los módulos
- `loop()`: Loop principal
- `changeMode(AppMode)`: Cambia entre modos
- `handleMenuNavigation()`, `handlePomodoroMode()`, etc.: Lógica de cada modo
- `updateUI()`: Actualiza la pantalla (se llama cada 300ms)

**Flujo de ejecución:**
1. `setup()` inicializa hardware, UI y modos
2. `loop()` actualiza inputs, sensores y outputs
3. Según `currentMode`, se llama al handler correspondiente
4. `updateUI()` refresca la pantalla cada 300ms

## Dependencias entre Módulos

```
main.cpp
  ├─> config/* (todos los archivos de configuración)
  ├─> hardware/Display
  ├─> hardware/Input
  ├─> hardware/Output
  ├─> hardware/Sensors
  ├─> hardware/Storage
  ├─> ui/Menu
  ├─> ui/Screens
  ├─> modes/Pomodoro
  └─> modes/Gym

hardware/Display
  ├─> config/pins
  ├─> config/colors
  └─> Adafruit ST7735/ST7789

hardware/Input
  └─> config/pins

hardware/Output
  ├─> config/pins
  └─> config/settings

hardware/Sensors
  ├─> config/pins
  └─> config/settings

hardware/Storage
  └─> SPIFFS/Preferences

ui/Menu
  ├─> hardware/Display
  ├─> hardware/Input
  └─> config/colors

ui/Screens
  ├─> hardware/Display
  ├─> hardware/Storage
  └─> config/colors

modes/Pomodoro
  ├─> hardware/Display
  ├─> hardware/Output
  ├─> hardware/Storage
  └─> config/settings

modes/Gym
  ├─> hardware/Display
  ├─> hardware/Output
  ├─> hardware/Sensors
  ├─> hardware/Storage
  └─> config/settings
```

## Guía de Modificación

### Cambiar Pines del Hardware
Editar `config/pins.h` y modificar las definiciones `PIN_*`.

### Cambiar Tiempos o Umbrales
Editar `config/settings.h` y modificar las constantes correspondientes.

### Cambiar Colores de la UI
Editar `config/colors.h` con los valores RGB565 deseados.

### Añadir un Nuevo Modo
1. Crear `modes/NuevoModo.h` y `modes/NuevoModo.cpp`
2. Definir la clase con los métodos: `begin()`, `update()`, `draw()`
3. Añadir el modo al enum `AppMode` en `main.cpp`
4. Crear el handler `handleNuevoModo()` en `main.cpp`
5. Añadir la opción al menú en `ui/Menu.cpp`
6. Añadir el caso en `changeMode()` y `updateUI()` en `main.cpp`

### Añadir un Nuevo Sensor
1. Añadir el pin en `config/pins.h`
2. Añadir constantes de configuración en `config/settings.h`
3. Implementar la lógica en `hardware/Sensors.cpp`
4. Exponer funciones públicas en `hardware/Sensors.h`

### Modificar la UI
- **Menú**: Editar `ui/Menu.cpp`, método `drawMainMenu()`
- **Pantallas estáticas**: Editar `ui/Screens.cpp`
- **Pantallas de modos**: Editar el método `draw()` del modo respectivo

### Añadir Sonidos al Buzzer
Editar `hardware/Output.cpp` y crear un nuevo método `playNuevoSonido()`:
```cpp
void Output::playNuevoSonido() {
  ledcWriteTone(0, FRECUENCIA_HZ);
  delay(DURACION_MS);
  ledcWriteTone(0, 0);  // Apagar
}
```

### Almacenar Nuevos Datos
1. Definir la estructura en `hardware/Storage.h`
2. Implementar métodos `load()` y `save()` en `hardware/Storage.cpp`
3. Usar SPIFFS o Preferences según el tipo de dato

## Convenciones de Código

### Naming Conventions
- **Constantes**: `SCREAMING_SNAKE_CASE` (ej: `LED_BLINK_FAST_MS`)
- **Clases**: `PascalCase` (ej: `PomodoroTimer`)
- **Funciones/Variables**: `camelCase` (ej: `getPotPosition`)
- **Enums**: `PascalCase` para el tipo, `SCREAMING_SNAKE_CASE` para valores
- **Pines**: `PIN_*` (ej: `PIN_LED_GREEN`)
- **Instancias globales**: lowercase (ej: `display`, `input`, `output`)

### Estructura de Archivos
- Cada módulo tiene su `.h` y `.cpp`
- Los `.h` tienen guards (`#ifndef MODULE_H`)
- Las instancias globales se declaran con `extern` en `.h` e instancian en `.cpp`

### Update Pattern
Todos los módulos que necesitan actualización periódica tienen:
```cpp
void begin();   // Inicialización (llamar en setup())
void update();  // Actualización (llamar en loop())
```

### Draw Pattern
Los módulos con UI implementan:
```cpp
void draw();    // Dibuja la interfaz
```

## Librerías Externas

Definidas en `platformio.ini`:

```ini
lib_deps =
    adafruit/Adafruit GFX Library@^1.11.9
    adafruit/Adafruit ST7735 and ST7789 Library@^1.10.3
    bblanchon/ArduinoJson@^6.21.4
    me-no-dev/AsyncTCP@^1.1.4
    me-no-dev/ESP Async WebServer@^1.2.4
    https://github.com/nayuki/QR-Code-generator.git
```

## Notas Importantes

### Conflictos de Nombres
- Las constantes de tiempo LED (`LED_BLINK_FAST_MS`, `LED_BLINK_SLOW_MS`) terminan en `_MS` para evitar conflictos con el enum `LEDState`
- No usar nombres que colisionen con las macros del preprocessor

### Memoria
- El ESP32 tiene memoria limitada
- Evitar usar `String`, preferir `char[]`
- Las imágenes/fuentes grandes deben estar en PROGMEM

### Timing
- `millis()` se usa para tiempos no bloqueantes
- Evitar `delay()` excepto en sonidos cortos
- El loop principal debe ejecutarse rápido (<50ms)

### Display
- Las coordenadas van de (0,0) en la esquina superior izquierda
- Ancho: 128px, Alto: 160px
- La orientación se puede cambiar con `setRotation()`

### PWM para Buzzer
- Se usa el canal 0 de LEDC
- Frecuencia resonante del TMB12A05: 2300 Hz
- Resolución: 8 bits

## Compilación y Upload

```bash
# Compilar
pio run

# Upload al ESP32
pio run --target upload

# Monitor serial
pio device monitor

# Limpiar build
pio run --target clean
```

## Troubleshooting

### Error: "use of deleted function 'Display::Display()'"
Este error ocurre cuando `Adafruit_ST7735` no se inicializa correctamente:
- **Causa**: `Adafruit_ST7735` no tiene constructor por defecto y debe inicializarse con parámetros (CS, DC, RST)
- **Solución**: Usar lista de inicialización en el constructor de Display:
```cpp
Display::Display() : tft(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST) {
}
```
- No intentar inicializar `tft` mediante asignación en el método `begin()`

### Error: "BUZZER_FREQ_MED is undefined"
- Verificar que `hardware/Output.h` incluye `../config/settings.h`
- Verificar que `BUZZER_FREQ_MED` está definido en `config/settings.h`

### Error: Conflicto de nombres LED_BLINK_FAST
- Asegurarse de usar `LED_BLINK_FAST_MS` para la constante de tiempo
- Usar `LED_BLINK_FAST` solo para el enum `LEDState`

### IntelliSense no encuentra archivos
- Recargar la ventana de VS Code
- Ejecutar: `pio run --target clean` y recompilar
- Verificar que `.vscode/c_cpp_properties.json` esté configurado correctamente

### Display no funciona
- Verificar conexiones de hardware
- Verificar pines en `config/pins.h`
- Verificar tipo de display (ST7735 vs ST7789)

### Botones no responden
- Verificar conexiones y pull-up/pull-down
- Ajustar debounce en `config/settings.h`

---

**Versión del documento**: 1.0
**Última actualización**: 2025-11-27
**Autor**: Claude Code
