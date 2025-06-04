/**
 * Actualización de la licencia para Heltec ESP32 LoRa WiFi v3.2
 * Utiliza el comando AT+CDKEY para actualizar la licencia del dispositivo
 */

#include <HardwareSerial.h>

// Definir el puerto serie para la comunicación con el módulo
HardwareSerial SerialAT(1); // UART1 en ESP32

// Clave de licencia proporcionada
const String licenseKey = "7C192B2E1E7CBB9154E4A74BD94055AB";

void setup()
{
    // Inicializar el puerto serial para monitoreo
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Esperar a que el puerto serie se conecte
    }

    Serial.println("=== Actualización de licencia para Heltec ESP32 LoRa WiFi v3.2 ===");
    delay(1000);

    // Inicializar el puerto serial para comunicarse con el módulo
    SerialAT.begin(115200, SERIAL_8N1, 41, 40); // RX, TX
    delay(500);

    // Probar comunicación básica
    Serial.println("\nVerificando comunicación con el módulo...");
    enviarComando("AT");
    delay(1000);

    // Actualizar la licencia con el comando AT+CDKEY
    Serial.println("\nActualizando licencia del dispositivo...");
    Serial.println("Enviando comando: AT+CDKEY=" + licenseKey);
    enviarComando("AT+CDKEY=" + licenseKey);

    Serial.println("\nProceso de actualización de licencia completado.");
    Serial.println("Si no ves respuesta 'OK', verifica la conexión y la clave de licencia.");
    Serial.println("\nPara probar manualmente, escribe 'AT+CDKEY=" + licenseKey + "' en el monitor serial.");
}

void loop()
{
    // Leer respuestas del módulo
    if (SerialAT.available())
    {
        String respuesta = SerialAT.readString();
        Serial.println("Respuesta: " + respuesta);
    }

    // Enviar comandos desde el monitor serial para pruebas adicionales
    if (Serial.available())
    {
        String comando = Serial.readStringUntil('\n');
        enviarComando(comando);
    }
}

// Función para enviar comandos AT y capturar respuestas
void enviarComando(String comando)
{
    Serial.println(">> Enviando: " + comando);
    SerialAT.println(comando);

    // Esperar y capturar respuesta
    delay(1000);
    String respuesta = "";
    while (SerialAT.available())
    {
        respuesta += (char)SerialAT.read();
    }

    if (respuesta.length() > 0)
    {
        Serial.println("<< Respuesta: " + respuesta);
    }
    else
    {
        Serial.println("<< Sin respuesta (verifica conexión o intenta de nuevo)");
    }
}