// devices.h
#ifndef DEVICES_H
#define DEVICES_H

#include "config.h"

// Base Device Class
class Device {
public:
    Device(int pin, const char* name) : pin(pin), name(name) {}

    virtual void setup() = 0;
    virtual void updateState() = 0;

    int pin;
    const char* name;
};

// Relay Device Class
class RelayDevice : public Device {
public:
    RelayDevice(int pin, const char* name) : Device(pin, name), state("OFF") {}

    void setup() override {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW); // Default state
    }

    void updateState() override {
        // No specific action for relays here; set state through methods
    }

    void setState(const char* newState) {
        if (strcmp(state, newState) != 0) {
            digitalWrite(pin, strcmp(newState, "ON") == 0 ? HIGH : LOW);
            state = newState;
        }
    }

    const char* getState() const {
        return state;
    }

private:
    const char* state;
};

// Moisture Meter Device Class
class MoistureDevice : public Device {
public:
    MoistureDevice(int pin, const char* name) : Device(pin, name), moisturePercentage(0) {}

    void setup() override {
        pinMode(pin, INPUT);
    }

    void updateState() override {
        int sensorValue = analogRead(pin);
        moisturePercentage = map(sensorValue, MOISTURE_METERS_MIN, MOISTURE_METERS_MAX, 0, 100);

        if (moisturePercentage < 0) moisturePercentage = 0;
        if (moisturePercentage > 100) moisturePercentage = 100;
    }

    int getMoisturePercentage() const {
        return moisturePercentage;
    }

private:
    int moisturePercentage;
};

#endif // DEVICES_H
