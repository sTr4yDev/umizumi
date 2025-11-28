import sounddevice as sd
import numpy as np
import serial
import time
import sys
import threading

# ===== CONFIGURACIÓN =====
SERIAL_PORT = 'COM5'
BAUD_RATE = 115200
SAMPLE_RATE = 44100
BLOCK_SIZE = 1024
THRESHOLD = 0.3
CLAP_COOLDOWN = 0.5

# ===== INICIALIZAR =====
print("========================================")
print("  DETECTOR DE CLAP - LAPTOP MIC V2")
print("========================================\n")

# Conectar Serial
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)
    print(f"✓ Conectado a ESP32 en {SERIAL_PORT}")
except Exception as e:
    print(f"✗ Error al conectar Serial: {e}")
    sys.exit(1)

print("✓ Librerías cargadas")
print(f"\nUmbral: {THRESHOLD}")
print("Haz CLAP cerca del micrófono!\n")
print("Presiona Ctrl+C para salir\n")

# ===== VARIABLES =====
last_clap_time = 0
clap_count = 0

# ===== LEER RESPUESTAS DEL ESP32 =====
def read_serial():
    while True:
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    print(f"\n[ESP32]: {line}")
        except:
            pass
        time.sleep(0.01)

# Iniciar thread para leer respuestas
thread = threading.Thread(target=read_serial, daemon=True)
thread.start()

# ===== CALLBACK DE AUDIO =====
def audio_callback(indata, frames, time_info, status):
    global last_clap_time, clap_count
    
    if status:
        print(f"Status: {status}")
    
    audio_data = indata[:, 0]
    level = np.sqrt(np.mean(audio_data**2))
    
    bar_length = int(level * 50)
    bar = '█' * min(bar_length, 50)
    print(f"\r{level:.3f} | {bar}                    ", end='', flush=True)
    
    current_time = time.time()
    if level > THRESHOLD and (current_time - last_clap_time) > CLAP_COOLDOWN:
        last_clap_time = current_time
        clap_count += 1
        
        print(f"\n\n>>> CLAP #{clap_count} DETECTADO! (nivel: {level:.3f})")
        print("Enviando 'CLAP' a ESP32...", end='', flush=True)
        
        try:
            ser.write(b'CLAP\n')
            ser.flush()
            print(" ✓")
        except Exception as e:
            print(f" ✗ Error: {e}")

# ===== LOOP PRINCIPAL =====
try:
    with sd.InputStream(callback=audio_callback, 
                        channels=1, 
                        samplerate=SAMPLE_RATE,
                        blocksize=BLOCK_SIZE):
        print("Nivel de audio:")
        while True:
            time.sleep(0.1)

except KeyboardInterrupt:
    print("\n\nDeteniendo...")

finally:
    ser.close()
    print("✓ Cerrado correctamente")


