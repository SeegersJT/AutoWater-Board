// devices.h
#ifndef DEVICES_H
#define DEVICES_H

#include "config.h"

// Base Device Class
class Device {
public:
    int pin;
    const char* name;

    Device(int pin, const char* name) : pin(pin), name(name) {}

    virtual void setup() = 0;
    virtual void updateState() = 0;
};

// Relay Device Class
class RelayDevice : public Device {
    private:
        const char* state;

    public:
        RelayDevice(int pin, const char* name) : Device(pin, name), state("OFF") {}

        void setup() override {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);
        }

        void updateState() override {

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
};

// Moisture Meter Device Class
class MoistureDevice : public Device {
    private:
        int moisturePercentage;
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
};

#endif // DEVICES_H
