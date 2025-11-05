# Smart Band ESP32 - Wokwi Simulation

Simulación de dispositivo IoT Smart Band con ESP32 para monitoreo de frecuencia cardíaca en tiempo real.

## Descripción del Proyecto

Este proyecto simula un dispositivo wearable (Smart Band) que:
- Lee datos de un sensor de pulso cardíaco
- Procesa y calcula la frecuencia cardíaca en bpm
- Envía los datos a una Edge API en Azure
- Controla un LED de alerta para valores anormales
- Registra toda la actividad en el Serial Monitor

## Componentes de Hardware

- ESP32
- Sensor de Pulso 
- LED de Alerta

## Arquitectura del Código

### Archivos Principales

#### Sketch.ino
Punto de entrada del programa. Inicializa y ejecuta el loop principal del dispositivo.

#### Device.h / Device.cpp
Clase principal que coordina todos los componentes:
- Inicialización del sistema
- Lectura de sensores
- Envío de datos
- Control de LED

#### EdgeServiceClient.h / EdgeServiceClient.cpp
Maneja la comunicación con el backend:
- Conexión WiFi
- Envío de datos HTTP POST
- Manejo de respuestas

#### PulseSimulator.h / PulseSimulator.cpp
Simula y lee el sensor de pulso:
- Lectura de voltaje del sensor
- Conversión ADC
- Modo de simulación para testing

#### HeartRateLog.h / HeartRateLog.cpp
Singleton para almacenar datos temporales:
- Smart Band ID
- Frecuencia cardíaca actual

#### Config.h
Configuración centralizada:
- Credenciales WiFi
- URL del backend
- Pines GPIO
- Umbrales de frecuencia cardíaca

## Configuración

### WiFi
```cpp
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
```

### HTTP POST Endpoint del Backend Edge API
```cpp
#define EDGE_BACKEND_URL "https://smart-band-edge-api-platform-grupo-uno.azurewebsites.net/api/v1/health-monitoring/data-records"
```

### Smart Band ID
```cpp
#define SMART_BAND_ID 1
```
Identificador único del dispositivo utilizado durante la simulación


### Control del LED

- LED ENCENDIDO: Frecuencia cardíaca menor a 60 bpm o mayor a 140 bpm
- LED APAGADO: Frecuencia cardíaca entre 60-140 bpm (rango normal)

### Formato de Datos Enviados

```json
{
  "smartBandId": 1,
  "pulse": "75"
}
```

## Logs en Serial Monitor

### Al Iniciar
```
========================================
Smart Band Edge Device - Starting
========================================
Initializing components...
I2C initialized
LED pin configured
========================================
WiFi Connection Starting
========================================
Connecting to WiFi.....
WiFi Connected Successfully
IP Address: 192.168.1.X
========================================
Device Ready - Starting measurements
========================================
```

### Durante Operación
```
----------------------------------------
Voltage Reading: 1.234 V
Calculated Heart Rate: 75 bpm

========================================
Sending Heart Rate Data to Edge API
========================================
Backend URL: https://...
Smart Band ID: 1
Heart Rate: 75 bpm
JSON Payload: {"smartBandId":1,"pulse":"75"}
----------------------------------------
Sending HTTP POST request...
========================================
Response from Edge API
========================================
HTTP Status Code: 201
Response Body: {...}
========================================

LED STATUS: OFF - Heart rate normal
```

## Respuesta del Backend

El backend clasifica automáticamente la frecuencia cardíaca:

- LOW: menor a 60 bpm
- NORMAL: 60-100 bpm
- HIGH: 101-120 bpm
- CRITICAL: mayor a 120 bpm

## Simulación en Wokwi

### Ejecutar la Simulación

1. Abrir el proyecto en Wokwi: https://wokwi.com/projects/446657341757965313
2. Click en el botón verde "Start Simulation"
3. Abrir el Serial Monitor para ver los logs
4. Ajustar el slider del sensor de pulso para simular diferentes frecuencias cardíacas

### Controles Interactivos

- Slider "Heart Rate": Ajusta el valor del pulso cardíaco simulado
- Observar el LED rojo: Se enciende cuando los valores están fuera del rango normal
- Serial Monitor: Muestra todos los logs de operación

## Arquitectura de Integración

```
[ESP32 Smart Band]
       |
       | WiFi
       v
[Azure App Service - Edge API]
       |
       v
[PostgreSQL Database - Supabase]
```

## Proyecto Relacionado

Este dispositivo IoT se conecta al Smart Band Edge API Platform:
- Repositorio: https://github.com/OscarGAV/SmartBand-Edge-Api-Platform
- Documentación Edge API: https://smart-band-edge-api-platform-grupo-uno.azurewebsites.net/docs