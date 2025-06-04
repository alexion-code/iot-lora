# Actualización de Licencia para Heltec ESP32 LoRa WiFi v3.2

Este documento explica cómo actualizar la licencia del módulo Heltec ESP32 LoRa WiFi v3.2 usando comandos AT.

## Requisitos

- Dispositivo Heltec ESP32 LoRa WiFi v3.2
- Arduino IDE o PlatformIO
- Cable USB
- Clave de licencia válida (ejemplo: `7C192B2E1E7CBB9154E4A74BD94055AB`)

## Métodos para actualizar la licencia

### Método 1: Usando el sketch Arduino proporcionado

1. Abre el archivo `Heltec_License_Update.ino` en Arduino IDE
2. Conecta tu dispositivo Heltec al puerto USB
3. Selecciona la placa correcta en Herramientas > Placa > ESP32 > Heltec WiFi LoRa 32 (V3)
4. Selecciona el puerto COM correcto (COM5 según tu configuración)
5. Sube el sketch al dispositivo
6. Abre el Monitor Serial a 115200 baudios
7. Verifica que recibas la respuesta "OK" después de enviar el comando AT+CDKEY

### Método 2: Usando comandos AT manualmente

1. Conecta tu dispositivo Heltec al puerto USB
2. Abre cualquiera de los ejemplos de comandos AT (`AT_Commands_Example.ino`)
3. Sube el sketch al dispositivo
4. Abre el Monitor Serial a 115200 baudios
5. Envía manualmente el comando: `AT+CDKEY=7C192B2E1E7CBB9154E4A74BD94055AB`
6. Verifica que recibas la respuesta "OK"

### Método 3: Usando esptool para flashear directamente

Si los métodos anteriores no funcionan, puedes flashear directamente el archivo binario de licencia:

1. Instala esptool: `pip install esptool`
2. Descarga el archivo binario de licencia de Heltec
3. Ejecuta el comando:
   ```
   esptool.py --chip esp32 --port COM5 --baud 921600 write_flash 0x10000 heltec_license.bin
   ```

## Verificación

Para verificar que la licencia se ha actualizado correctamente:

1. Reinicia el dispositivo
2. Envía el comando AT: `AT+CDEVINFO`
3. La respuesta debería incluir información sobre la licencia activada

## Solución de problemas

- **No hay respuesta a los comandos AT**: Verifica los pines RX/TX (41/40 para UART1)
- **Error en la actualización**: Verifica que la clave de licencia sea correcta
- **Dispositivo no responde**: Prueba con un reset físico (pulsa el botón RST)

## Notas adicionales

- La clave de licencia es única para cada dispositivo
- Este proceso solo debe realizarse una vez
- Si necesitas soporte adicional, contacta a Heltec en support@heltec.cn 