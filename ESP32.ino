#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "Ubidots.h"
#include <PubSubClient.h>
 #include <AD8232.h>

// Ubidots account details
#define UBIDOTS_TOKEN "YOUR_UBIDOTS_TOKEN"
#define UBIDOTS_WIFI_SSID "DR_HEMANT_WIFI"
#define UBIDOTS_DEVICE_LABEL "esp32"
#define UBIDOTS_VARIABLE_LABEL "sensor"

// Initialize AD8232
AD8232 ad8232(23, 22); // Pin 14 = LO-, Pin 27 = OUTPUT

// Initialize Ubidots client
Ubidots client(UBIDOTS_TOKEN, UBI_HTTP);

// Initialize Wi-Fi client
WiFiClientSecure wifiClient;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(UBIDOTS_WIFI_SSID, UBIDOTS_WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to Ubidots
  client.setDebug(true);
  client.setDeviceLabel(UBIDOTS_DEVICE_LABEL);
  client.addContext(UBIDOTS_VARIABLE_LABEL, "ESP32");
  client.wifiConnect(wifiClient);
  Serial.println("Connected to Ubidots");
}

void loop() {
  // Read AD8232 data
  float ecg = ad8232.readECG();

  // Send data to Ubidots
  client.add(UBIDOTS_VARIABLE_LABEL, ecg);
  client.sendAll();
  Serial.print("Sent ECG value: ");
  Serial.println(ecg);

  // Wait for 1 second
  delay(1000);
}
