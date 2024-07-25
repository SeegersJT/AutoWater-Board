#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <string>
#include "secrets.h"
#include "config.h"
#include "devices.h"
#include "display.cpp"

using display = DisplayWrapper;

MoistureDevice moistureMeter(MOISTURE_METER_PIN_MM01, "MOISTURE METER 01");
RelayDevice relay(RELAY_PIN_R01, "RELAY 01");

// =========================================================
// START UP
// =========================================================

void startDisplay() {
  display::init();
  display("Starting...").print();
  delay(5000);
}

void connectToWiFi() {
  display("Connecting WiFi").clear().print();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    display("Connecting...").bottom().print();
  }

  display("Connected").clear().print();
  display(WiFi.localIP().toString().c_str()).bottom().print();

  delay(2000);
}

void setupDevices() {
  moistureMeter.setup();
  relay.setup();
}

// =========================================================
// OUTPUTS - Serial print terminal data.
// =========================================================

void outputMoistureMeterData(const MoistureDevice& moistureDevice) {
  // "MOISTURE METER - [DEVICE_NAME] : [PERCENTAGE]%"
  Serial.print("MOISTURE METER - ");
  Serial.print(moistureDevice.name);
  Serial.print(" : ");
  Serial.print(moistureDevice.getMoisturePercentage());
  Serial.println("%");
}

void outputRelayState(const RelayDevice& relayDevice) {
  // "RELAY - [DEVICE_NAME] : [STATE]"
  Serial.print("RELAY - ");
  Serial.print(relayDevice.name);
  Serial.print(" : ");
  Serial.println(relayDevice.getState());
}

// =========================================================
// CONNECTORS - Logic to control relay based on moisture meter
// =========================================================

void connectRelayToMoistureMeter(RelayDevice& relayDevice, MoistureDevice& moistureDevice) {
  moistureDevice.updateState();

  int moisturePercentage = moistureDevice.getMoisturePercentage();

  if (moisturePercentage < MOISTURE_METERS_THRESHOLD) {
    relayDevice.setState("ON");
  } else {
    relayDevice.setState("OFF");
  }

  display("Moisture: " + String(moisturePercentage) + "%").print();
  display("Relay: " + String(relayDevice.getState())).bottom().print();
}

// =========================================================
// SETUP - Executed code at the start of the run
// =========================================================

void setup() {
  Serial.begin(9600);

  startDisplay();
  connectToWiFi();
  setupDevices();
}

// =========================================================
// LOOP - Executed code in ipAddress loop
// =========================================================

void loop() {
  connectRelayToMoistureMeter(relay, moistureMeter);

  delay(1000);
}
