#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <RadioLib.h>
#include <ArduinoJson.h>

// SX1262 Module connections
#define LORA_MOSI        10
#define LORA_MISO        11
#define LORA_SCK         9
#define LORA_CS          8
#define LORA_DIO1        5
#define LORA_RESET       6
#define LORA_BUSY        7

// Create instance of SX1262 radio
SX1262 radio = new Module(LORA_CS, LORA_DIO1, LORA_RESET, LORA_BUSY);

// WiFi AP credentials
const char* AP_SSID = "ESP32_LoRaWAN_Config";
const char* AP_PASS = "password123";

// Web server running on port 80
WebServer server(80);

// Configuration structure
struct Config {
  char networkServer[64];
  char password[32];
  char deviceEUI[17];
  char appEUI[17];
  char appKey[33];
} config;

// EEPROM size
#define EEPROM_SIZE 256

void setup() {
  Serial.begin(115200);
  
  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  loadConfig();

  // Initialize SX1262
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(868.0, 125.0, 9, 7, 0x34, 20);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // Start WiFi Access Point
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Setup web server routes
  setupWebServer();
  server.begin();
}

void loop() {
  server.handleClient();
  
  // Handle LoRa operations here
  static unsigned long lastTransmission = 0;
  if (millis() - lastTransmission > 30000) {  // Send every 30 seconds
    sendRandomData();
    lastTransmission = millis();
  }
}

void setupWebServer() {
  // Handle CORS
  server.enableCORS(true);
  
  // Route for getting current config
  server.on("/config", HTTP_GET, []() {
    String response = "{";
    response += "\"networkServer\":\"" + String(config.networkServer) + "\",";
    response += "\"deviceEUI\":\"" + String(config.deviceEUI) + "\",";
    response += "\"appEUI\":\"" + String(config.appEUI) + "\",";
    response += "\"appKey\":\"" + String(config.appKey) + "\"";
    response += "}";
    server.send(200, "application/json", response);
  });

  // Route for updating config
  server.on("/config", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      String body = server.arg("plain");
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, body);
      
      strlcpy(config.networkServer, doc["networkServer"] | "", sizeof(config.networkServer));
      strlcpy(config.password, doc["password"] | "", sizeof(config.password));
      strlcpy(config.deviceEUI, doc["deviceEUI"] | "", sizeof(config.deviceEUI));
      strlcpy(config.appEUI, doc["appEUI"] | "", sizeof(config.appEUI));
      strlcpy(config.appKey, doc["appKey"] | "", sizeof(config.appKey));
      
      saveConfig();
      server.send(200, "application/json", "{\"status\":\"success\"}");
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No data received\"}");
    }
  });

  // Route for device status
  server.on("/status", HTTP_GET, []() {
    String status = "{";
    status += "\"rssi\":" + String(radio.getRSSI()) + ",";
    status += "\"snr\":" + String(radio.getSNR()) + ",";
    status += "\"frequency\":868.0";
    status += "}";
    server.send(200, "application/json", status);
  });
}

void loadConfig() {
  EEPROM.get(0, config);
  // Set defaults if empty
  if (strlen(config.networkServer) == 0) {
    strcpy(config.networkServer, "TELCO_TE_2G");
    strcpy(config.password, "teleco1234");
  }
}

void saveConfig() {
  EEPROM.put(0, config);
  EEPROM.commit();
}

void sendRandomData() {
  if (strlen(config.deviceEUI) == 0 || strlen(config.appEUI) == 0 || strlen(config.appKey) == 0) {
    Serial.println("LoRaWAN not configured");
    return;
  }

  // Generate random number
  int randomNumber = random(1, 100);
  
  // Prepare packet
  byte packet[2];
  packet[0] = (randomNumber >> 8) & 0xFF;
  packet[1] = randomNumber & 0xFF;

  // Send packet
  int state = radio.transmit(packet, 2);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("Transmission successful!"));
    Serial.print(F("Data sent: "));
    Serial.println(randomNumber);
  } else {
    Serial.print(F("Transmission failed, code "));
    Serial.println(state);
  }
}