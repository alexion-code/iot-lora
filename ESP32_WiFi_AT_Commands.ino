/**
 * Ejemplo de comandos AT para WiFi en ESP32 con Heltec WiFi LoRa 32 v3
 */

void setup()
{
    // Inicializar comunicación serial
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Esperar a que el puerto serie se conecte
    }

    // Inicializar el ESP32 en modo AT
    Serial.println("Configurando ESP32 para usar comandos AT de WiFi");

    // Pequeña pausa antes de empezar
    delay(1000);

    // --- Ejemplos de comandos AT para WiFi ---

    // Verificar comunicación
    Serial.println("\n--- Probando comunicación básica ---");
    Serial.println("Comando: AT");
    Serial.println("Respuesta esperada: OK");

    // Listar redes WiFi disponibles
    Serial.println("\n--- Escanear redes WiFi ---");
    Serial.println("Comando: AT+CWLAP");
    Serial.println("Respuesta: Lista de puntos de acceso");

    // Conectar a una red WiFi
    Serial.println("\n--- Conectar a WiFi ---");
    Serial.println("Comando: AT+CWJAP=\"nombre_red\",\"contraseña\"");
    Serial.println("Respuesta: OK o ERROR");

    // Verificar dirección IP
    Serial.println("\n--- Verificar IP ---");
    Serial.println("Comando: AT+CIFSR");
    Serial.println("Respuesta: Información de IP");

    // Configurar como cliente TCP
    Serial.println("\n--- Configurar conexión TCP ---");
    Serial.println("Comando: AT+CIPSTART=\"TCP\",\"ejemplo.com\",80");
    Serial.println("Respuesta: CONNECT OK o ERROR");

    // Enviar datos
    Serial.println("\n--- Enviar datos ---");
    Serial.println("Comando: AT+CIPSEND=<longitud_datos>");
    Serial.println("Respuesta: > (después ingresar los datos)");

    // Cerrar conexión
    Serial.println("\n--- Cerrar conexión ---");
    Serial.println("Comando: AT+CIPCLOSE");
    Serial.println("Respuesta: CLOSED OK");

    Serial.println("\n--- Instrucciones de uso ---");
    Serial.println("1. Conecta un módulo compatible con comandos AT a los pines de comunicación serial");
    Serial.println("2. En el monitor serial, puedes ingresar los comandos AT directamente");
    Serial.println("3. Observa las respuestas del módulo");
}

void loop()
{
    // Enviar datos del monitor serial al módulo AT
    if (Serial.available())
    {
        String comando = Serial.readStringUntil('\n');
        Serial.print(">> ");
        Serial.println(comando);
        Serial2.println(comando);
    }

    // Recibir datos del módulo AT y mostrarlos en el monitor serial
    if (Serial2.available())
    {
        String respuesta = Serial2.readString();
        Serial.println(respuesta);
    }

    delay(100);
}