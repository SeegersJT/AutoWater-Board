#include <Arduino.h>
#include "wifi_manager.h"
#include "config_manager.h"
#include "device_wrapper.h"
#include "display_wrapper.h"

using display = DisplayWrapper;

DeviceWrapper<MoistureDevice>* moistureSensor_MM01;
DeviceWrapper<RelayDevice>* relay_R01;

// =========================================================
// START UP
// =========================================================

void startDisplay() {
    display::init();
    display("Starting...").print();
    delay(5000);
}

void setupDevices() {
    moistureSensor_MM01 = new DeviceWrapper<MoistureDevice>(MOISTURE_SENSOR_PIN_MM01, "MOISTURE SENSOR 01");
    relay_R01 = new DeviceWrapper<RelayDevice>(RELAY_PIN_R01, "RELAY 01");

    moistureSensor_MM01->setup();
    relay_R01->setup();
}

// =========================================================
// CONNECTORS - Logic to control relay based on moisture sensor
// =========================================================

void connectRelayToMoistureSensor(DeviceWrapper<RelayDevice>& relayDevice, DeviceWrapper<MoistureDevice>& moistureDevice) {
    unsigned long currentMillis = millis();

    if (currentMillis - moistureDevice.getTimestamp() >= MOISTURE_SENSORS_CHECK_INTERVAL) {
        int moisturePercentage = moistureDevice.getMoisturePercentage();

        if (moisturePercentage < ACTIVATE_RELAY_THRESHOLD) {
            relayDevice.setState("ON");
        } else {
            relayDevice.setState("OFF");
        }

        display("Moisture: " + String(moisturePercentage) + "%").print();
        display("Relay: " + String(relayDevice.getState())).bottom().print();
    }

    if (relayDevice.getState() == "ON" && currentMillis - moistureDevice.getTimestamp() >= RELAY_ON_DURATION) {
        relayDevice.setState("OFF");
    }
}

// =========================================================
// SETUP - Executed code at the start of the run
// =========================================================

void setup() {
    Serial.begin(9600);

    startDisplay();

    readConfig();
    
    setupDevices();

    WiFiManager::connectToWiFi();
}

// =========================================================
// LOOP - Executed code in ipAddress loop
// =========================================================

void loop() {
    connectRelayToMoistureSensor(*relay_R01, *moistureSensor_MM01);

    delay(1000);
}
