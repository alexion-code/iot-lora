#include <Arduino.h>
#include <Heltec.h>
#include <LoRaWan_APP.h>

// Definiciones de la placa Heltec WiFi LoRa 32 V3
#define BOARD_HELTEC_WIFI_LORA_32_V3 1
#define SLOW_CLK_TPYE 0

/* OTAA para*/
uint8_t devEui[] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x07, 0x02, 0x37};
uint8_t appEui[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t appKey[] = {0x64, 0xB6, 0x63, 0x45, 0x98, 0xE2, 0x83, 0xB6, 0x6B, 0x91, 0x1E, 0xA1, 0x87, 0xDC, 0x4D, 0xD1};

/* ABP para*/
uint8_t nwkSKey[] = {0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda, 0x85};
uint8_t appSKey[] = {0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef, 0x67};
uint32_t devAddr = (uint32_t)0x007e6ae1;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = {0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

/*LoraWan region, select in arduino IDE tools*/
#define ACTIVE_REGION LORAMAC_REGION_EU868
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP*/
bool overTheAirActivation = true;

/*ADR enable*/
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;

/* Application port */
uint8_t appPort = 2;

/* Number of trials to transmit the frame */
uint8_t confirmedNbTrials = 4;

/* Variable para almacenar el dato float */
float sensorValue = 25.75; // Ejemplo de valor float

/* Prepares the payload of the frame */
static void prepareTxFrame(uint8_t port)
{
    // Convertir el float a bytes
    uint8_t *floatBytes = (uint8_t *)&sensorValue;

    // El tamaño de un float es 4 bytes
    appDataSize = 4;

    // Copiar los bytes del float al buffer de datos
    for (int i = 0; i < 4; i++)
    {
        appData[i] = floatBytes[i];
    }
}

void setup()
{
    Serial.begin(115200);
    Heltec.begin(true /*DisplayEnable*/, false /*LoRaEnable*/, true /*SerialEnable*/);
}

void loop()
{
    switch (deviceState)
    {
    case DEVICE_STATE_INIT:
    {
#if (LORAWAN_DEVEUI_AUTO)
        printf("LORAWAN_DEVEUI_AUTO");
        LoRaWAN.generateDeveuiByChipID();
#endif
        LoRaWAN.init(loraWanClass, loraWanRegion);
        // both set join DR and DR when ADR off
        LoRaWAN.setDefaultDR(3);
        break;
    }
    case DEVICE_STATE_JOIN:
    {
        LoRaWAN.join();
        break;
    }
    case DEVICE_STATE_SEND:
    {
        prepareTxFrame(appPort);
        LoRaWAN.send();
        deviceState = DEVICE_STATE_CYCLE;
        break;
    }
    case DEVICE_STATE_CYCLE:
    {
        // Schedule next packet transmission
        txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
        LoRaWAN.cycle(txDutyCycleTime);
        deviceState = DEVICE_STATE_SLEEP;
        break;
    }
    case DEVICE_STATE_SLEEP:
    {
        LoRaWAN.sleep(loraWanClass);
        break;
    }
    default:
    {
        deviceState = DEVICE_STATE_INIT;
        break;
    }
    }
}