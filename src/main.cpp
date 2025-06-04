#include <Arduino.h>
#include <math.h>  // Necesario para roundf()
/*
#ifndef GPIO_IS_VALID_GPIO
  // Definición para el ESP32: consideramos válido el pin si es menor que 40.
  #define GPIO_IS_VALID_GPIO(pin) ((pin) < 40)
#endif
*/
/*#include <OneWire.h>*/
/*#include <DallasTemperature.h>*/
#include "WiFi.h"
#include "images.h"
#include "LoRaWan_APP.h"  // Asegúrate de que esta librería y sus dependencias estén correctamente instaladas
#include <Wire.h>  
#include "HT_SSD1306Wire.h"

// Configuración del bus OneWire y sensor Dallas en el pin 33 (ajusta según tu hardware)
/* OneWire ourWire(33);
DallasTemperature sensors(&ourWire); */

// Parámetros LoRaWAN
// OTAA
uint8_t devEui[] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x07, 0x0A, 0xDD};
uint8_t appEui[] = {0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77};
uint8_t appKey[] = {0x12, 0x50, 0xD4, 0x50, 0x83, 0x03, 0x4D, 0x4F, 0xA9, 0x79, 0x03, 0x55, 0xA9, 0x43, 0xFE, 0x82};
// uint8_t appKey[] = {0xEC, 0x4C, 0x6B, 0xA7, 0x2C, 0xCF, 0x6A, 0xDD, 0x4C, 0xCB, 0xF8, 0x29, 0xE4, 0x88, 0x90, 0x84};
// ABP
uint8_t nwkSKey[]   = { 0x15, 0xB1, 0xD0, 0xEF, 0xA4, 0x63, 0xDF, 0xBE, 0x3D, 0x11, 0x18, 0x1E, 0x1E, 0xC7, 0xDA, 0x85 };
uint8_t appSKey[]   = { 0xD7, 0x2C, 0x78, 0x75, 0x8C, 0xDC, 0xCA, 0xBF, 0x55, 0xEE, 0x4A, 0x77, 0x8D, 0x16, 0xEF, 0x67 };
uint32_t devAddr    = (uint32_t)0x007e6ae1;

// Channels mask: habilitamos los canales 0-7
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

// Región LoRaWAN según la configuración del Arduino IDE
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

// Clase LoRaWAN (usamos CLASS_A para bajo consumo)
DeviceClass_t loraWanClass = CLASS_A;

// Ciclo de transmisión en milisegundos
uint32_t appTxDutyCycle = 15000;

// Selección OTAA o ABP
bool overTheAirActivation = true;

// ADR habilitado
bool loraWanAdr = true;

// Transmisiones confirmadas
bool isTxConfirmed = true;

// Puerto de la aplicación
uint8_t appPort = 2;

// Número de intentos de envío confirmados
uint8_t confirmedNbTrials = 4;

// Variables globales definidas en LoRaWan_APP.h (dejamos las que no generan conflicto)
extern uint8_t appData[];
extern uint8_t appDataSize;
extern uint32_t txDutyCycleTime;
// La variable 'deviceState' ya está definida en la librería.

// Función para preparar el frame a transmitir usando un valor float redondeado a 2 decimales.
static void prepareTxFrame(uint8_t port) {
  // Solicita la lectura del sensor DS18B20
  // sensors.requestTemperatures();
  float tempC = randr(15,50); // sensors.getTempCByIndex(0);
  
  // Redondea la temperatura a 2 decimales: 
  // Por ejemplo, una lectura de 23.456 se redondeará a 23.46.
  tempC = roundf(tempC * 100.0) / 100.0;
  
  // Depuración: muestra la temperatura redondeada en el monitor serie
  Serial.print("Temperatura en C (flotante, 2 decimales): ");
  Serial.println(tempC, 2);
  
  // Usamos una unión para convertir el float en 4 bytes (representación IEEE 754)
  union {
    float f;
    uint8_t b[4];
  } floatUnion;
  
  floatUnion.f = tempC;
  
  // Empaqueta los 4 bytes en el array appData.
  appDataSize = 4;
  appData[0] = floatUnion.b[0];
  appData[1] = floatUnion.b[1];
  appData[2] = floatUnion.b[2];
  appData[3] = floatUnion.b[3];
  Serial.println("enviando...");
}

SSD1306Wire  factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst


void logo(){
	factory_display.clear();
	factory_display.drawXbm(0,5,logo_width,logo_height,(const unsigned char *)logo_bits);
	factory_display.display();
}
void VextON(void)
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
  
}

void VextOFF(void) //Vext default OFF
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}
void setup() {
  Serial.begin(115200);
  VextON();
	delay(100);
	factory_display.init();
	factory_display.clear();
	factory_display.display();
	logo();
	delay(300);
	factory_display.clear();
  // sensors.begin();   // Inicializa el sensor Dallas
  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);  // Inicializa la placa Heltec
}

void loop() {
  // La máquina de estados es gestionada por la librería LoRaWan_APP.h.
  switch (deviceState) {
    case DEVICE_STATE_INIT: {
      factory_display.clear();
		  factory_display.drawString(0, 0, "DEVICE_STATE_INIT");
		  factory_display.display();
      delay(500);
      #if (LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();
      #endif
      LoRaWAN.init(loraWanClass, loraWanRegion);
      LoRaWAN.setDefaultDR(3);
      break;
    }
    case DEVICE_STATE_JOIN: {
      factory_display.clear();
		  factory_display.drawString(0, 0, "DEVICE_STATE_JOIN");
		  factory_display.display();
      delay(500);
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND: {
      // Prepara y envía el frame con el valor float redondeado a 2 decimales
      factory_display.clear();
		  factory_display.drawString(0, 0, "Data: "+String(appData[0])+String(appData[1])+String(appData[2])+String(appData[3]));
		  factory_display.display();
      delay(1000);
      prepareTxFrame(appPort);
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE: {
      txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP: {
      LoRaWAN.sleep(loraWanClass);
      break;
    }
    default: {
      factory_display.clear();
		  factory_display.drawString(0, 0, "DEVICE_STATE_INIT");
		  factory_display.display();
      delay(500);
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}
