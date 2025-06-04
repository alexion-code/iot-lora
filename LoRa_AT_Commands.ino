/**
 * Ejemplo de comandos AT para módulo LoRa en Heltec WiFi LoRa 32 v3
 */

#include <HardwareSerial.h>

// Puerto serial para comunicarse con el módulo LoRa
HardwareSerial LoRaSerial(1); // UART1 en ESP32

void setup()
{
    // Inicializar puerto serial para monitor
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Esperar a que el puerto serie se conecte
    }

    // Inicializar puerto serial para comunicarse con el módulo LoRa
    LoRaSerial.begin(115200, SERIAL_8N1, 41, 40); // RX, TX

    Serial.println("=== Comandos AT para LoRa ===");
    Serial.println("Enviando comando de prueba...");

    // Enviar comando AT básico
    enviarComando("AT");
    delay(500);

    // Mostrar información sobre comandos LoRa comunes
    mostrarComandosLoRa();
}

void loop()
{
    // Procesar comandos desde monitor serial
    if (Serial.available())
    {
        String comando = Serial.readStringUntil('\n');
        enviarComando(comando);
    }

    // Leer respuestas del módulo
    if (LoRaSerial.available())
    {
        String respuesta = LoRaSerial.readString();
        Serial.println("< " + respuesta);
    }
}

// Enviar un comando AT al módulo LoRa
void enviarComando(String comando)
{
    Serial.println("> " + comando);
    LoRaSerial.println(comando);
    delay(100);
}

// Muestra información sobre comandos AT comunes para LoRa
void mostrarComandosLoRa()
{
    Serial.println("\n=== Comandos AT comunes para LoRa ===");
    Serial.println("AT                    - Comprobar comunicación");
    Serial.println("AT+VER                - Obtener versión del firmware");
    Serial.println("AT+RESET              - Reiniciar el módulo");
    Serial.println("AT+PARAMETER=<SF>,<BW>,<CR>,<Preamble>,<Power> - Configurar parámetros LoRa");
    Serial.println("  SF: Factor de propagación (7-12)");
    Serial.println("  BW: Ancho de banda (0:125kHz, 1:250kHz, 2:500kHz)");
    Serial.println("  CR: Tasa de codificación (1:4/5, 2:4/6, 3:4/7, 4:4/8)");
    Serial.println("  Preamble: Longitud del preámbulo (8-65535)");
    Serial.println("  Power: Potencia de transmisión (0-15)");
    Serial.println("AT+BAND=<Freq>        - Configurar frecuencia en Hz (ej: 868100000)");
    Serial.println("AT+ADDRESS=<Addr>     - Configurar dirección del nodo (0-65535)");
    Serial.println("AT+NETWORKID=<NID>    - Configurar ID de red (0-16)");
    Serial.println("AT+SEND=<Addr>,<Longitud>,<Datos> - Enviar datos");
    Serial.println("AT+RECV               - Recibir datos");
    Serial.println("\nEscribe los comandos en el monitor serial para enviarlos al módulo LoRa");
}