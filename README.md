# Heltec LoRa 32 ESP32 V3 LoRaWAN Project

Este proyecto implementa la comunicación LoRaWAN usando el módulo Heltec LoRa 32 ESP32 V3.

## Requisitos

- PlatformIO IDE (recomendado) o Arduino IDE
- Heltec LoRa 32 ESP32 V3
- Conexión a una red LoRaWAN (como The Things Network)

## Configuración

1. Clona este repositorio
2. Abre el proyecto en PlatformIO
3. Configura tus credenciales LoRaWAN en `src/main.cpp`:
   - Para OTAA: Actualiza `devEui`, `appEui`, y `appKey`
   - Para ABP: Actualiza `nwkSKey`, `appSKey`, y `devAddr`

## Características

- Soporte para LoRaWAN Class A
- Activación OTAA (Over-The-Air Activation)
- Envío de datos cada 15 segundos
- Mensajes confirmados
- Adaptación de tasa de datos (ADR) habilitada

## Notas

- El proyecto usa las librerías oficiales de Heltec Automation
- La región LoRaWAN se configura automáticamente según la configuración en PlatformIO
- Los datos de ejemplo se envían en el puerto 2 