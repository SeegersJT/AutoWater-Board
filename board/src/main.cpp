#include <Arduino.h>
#include "wifi_manager.h"
#include "globals.h"
#include "device_wrapper.h"

DeviceWrapper<MoistureDevice>* moistureSensor_MM01;
DeviceWrapper<RelayDevice>* relay_R01;

void listSPIFFSFiles() {
  Serial.println("Listing SPIFFS files:");
  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while (file) {
    Serial.print("FILE: ");
    Serial.print(file.name());
    Serial.print(" - SIZE: ");
    Serial.println(file.size());
    file = root.openNextFile();
  }

  Serial.println("");
}

// =========================================================
// START UP
// =========================================================

void startDisplay() {
  display::init();
  display("Starting...").print();
  delay(5000);
}

void setupDevices() {
  display("Device Setup").clear().print();
  display("Initiating...").bottom().print();
  delay(2000);

  moistureSensor_MM01 = new DeviceWrapper<MoistureDevice>(config["MOISTURE_SENSOR_PIN_MM01"], "MOISTURE SENSOR 01");
  relay_R01 = new DeviceWrapper<RelayDevice>(config["RELAY_PIN_R01"], "RELAY 01");

  moistureSensor_MM01->setup();
  relay_R01->setup();

  display("Device Setup").clear().print();
  display("Successfull").bottom().print();
  delay(2000);
}

// =========================================================
// CONNECTORS - Logic to control relay based on moisture sensor
// =========================================================

void connectRelayToMoistureSensor(DeviceWrapper<RelayDevice>& relayDevice, DeviceWrapper<MoistureDevice>& moistureDevice) {
  unsigned long currentMillis = millis();

  if (currentMillis - moistureDevice.getTimestamp() >= config["MOISTURE_SENSORS_INTERVAL_MINUTES"] * 60 * 1000) {
    int moisturePercentage = moistureDevice.getMoisturePercentage();

    if (moisturePercentage < config["ACTIVATE_RELAY_THRESHOLD"]) {
      relayDevice.setState("ON");
    } else {
      relayDevice.setState("OFF");
    }

    display("Moisture: " + String(moisturePercentage) + "%").clear().print();
    display("Relay: " + String(relayDevice.getState())).bottom().print();
  }

  if (relayDevice.getState() == "ON" && currentMillis - moistureDevice.getTimestamp() >= config["RELAY_ON_DURATION"]) {
    relayDevice.setState("OFF");
  }
}

// =========================================================
// SETUP - Executed code at the start of the run
// =========================================================

void setup() {
  Serial.begin(9600);

  startDisplay();

  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }

  listSPIFFSFiles();

  config.initialConfig();
  config.readFromINI();

  setupDevices();

  WiFiManager::setHostname();

  WiFiManager::connectToWiFi();

  WiFiManager::enableAccessPoint();
}

// =========================================================
// LOOP - Executed code in ipAddress loop
// =========================================================

void loop() {
  connectRelayToMoistureSensor(*relay_R01, *moistureSensor_MM01);

  display::checkBacklight();

  WiFiManager::accessPointListen();

  delay(100);
}
