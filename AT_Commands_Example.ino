/**
 * Ejemplo de comandos AT para Arduino con Heltec WiFi LoRa 32 v3
 */

#include <HardwareSerial.h>

// Definir el puerto serie que se usará para comunicarse con el módulo (Serial1, Serial2, etc.)
// En Heltec ESP32 v3, puedes usar Serial1 (pines 41/RX y 40/TX)
HardwareSerial SerialAT(1);

void setup()
{
    // Inicializar el puerto serial para el monitor
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Esperar a que el puerto serie se conecte
    }

    // Inicializar el puerto serial para comunicarse con el módulo AT
    SerialAT.begin(115200, SERIAL_8N1, 41, 40); // RX, TX

    Serial.println("Iniciando comunicación con comandos AT");
    delay(1000);

    // Enviar comando AT básico para probar la comunicación
    enviarComandoAT("AT");
    delay(1000);

    // Obtener información de versión
    enviarComandoAT("AT+GMR");
    delay(1000);
}

void loop()
{
    // Leer respuestas del módulo
    if (SerialAT.available())
    {
        String respuesta = SerialAT.readString();
        Serial.println("Respuesta: " + respuesta);
    }

    // Enviar comandos desde el monitor serial
    if (Serial.available())
    {
        String comando = Serial.readStringUntil('\n');
        enviarComandoAT(comando);
    }
}

// Función para enviar comandos AT y mostrar la respuesta
void enviarComandoAT(String comando)
{
    Serial.println("Enviando: " + comando);
    SerialAT.println(comando);

    // Esperar y recoger la respuesta
    delay(500);
    String respuesta = "";
    while (SerialAT.available())
    {
        respuesta += (char)SerialAT.read();
    }

    if (respuesta.length() > 0)
    {
        Serial.println("Respuesta: " + respuesta);
    }
}