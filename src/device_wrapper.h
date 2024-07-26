// devices.h
#ifndef DEVICE_WRAPPER_H
#define DEVICE_WRAPPER_H

#include "config_manager.h"
#include <string>

// Base Device Class
class Device {
public:
    int pin;
    String name;
    unsigned long timestamp;

    Device(int pin, String name) {
        this->pin = pin;
        this->name = name;
        this->timestamp = 0;
    }

    virtual void setup() = 0;
    virtual void updateState() = 0;

    unsigned long getTimestamp() const {
        return this->timestamp;
    }

    void setTimestamp(unsigned long time) {
        this->timestamp = time;
    }
};

// Moisture Sensor Device Class
class MoistureDevice : public Device {
    private:
        int moisturePercentage;
    public:
        MoistureDevice(int pin, String name) : Device(pin, name), moisturePercentage(0) {}

        void setup() override {
            pinMode(pin, INPUT);
        }

        void updateState() override {
            int sensorValue = analogRead(pin);
            moisturePercentage = map(sensorValue, MOISTURE_SENSORS_MIN, MOISTURE_SENSORS_MAX, 100, 0);

            if (moisturePercentage < 0) moisturePercentage = 0;
            if (moisturePercentage > 100) moisturePercentage = 100;
        }

        int getMoisturePercentage() const {
            return moisturePercentage;
        }
};

// Relay Device Class
class RelayDevice : public Device {
    private:
        String state;

    public:
        RelayDevice(int pin, String name) : Device(pin, name), state("OFF") {}

        void setup() override {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);
        }

        void updateState() override {

        }

        void setState(String newState) {
            if (state != newState) {
                digitalWrite(pin, newState == "ON" ? HIGH : LOW);
                state = newState;
            }
        }

        String getState() const {
            return state;
        }
};



template<typename T>
class DeviceWrapper {
private:
    T* device;

public:
    DeviceWrapper(int pin, String name) : device(new T(pin, name)) {}

    ~DeviceWrapper() {
        delete device;
    }

    DeviceWrapper& setup() {
        device->setup();
        device->setTimestamp(millis());
        return *this;
    }

    DeviceWrapper& update() {
        device->updateState();
        device->updateTimestamp();
        return *this;
    }

    template<typename U = T>
    typename std::enable_if<std::is_same<U, RelayDevice>::value, DeviceWrapper&>::type setState(String newState) {
        device->setState(newState);
        device->setTimestamp(millis());
        return *this;
    }

    template<typename U = T>
    typename std::enable_if<std::is_same<U, RelayDevice>::value, String>::type getState() const {
        return device->getState();
    }

    template<typename U = T>
    typename std::enable_if<std::is_same<U, MoistureDevice>::value, int>::type getMoisturePercentage() const {
        device->updateState();
        device->setTimestamp(millis());
        return device->getMoisturePercentage();
    }

    unsigned long getTimestamp() const {
        return device->getTimestamp();
    }

    DeviceWrapper& setTimestamp(unsigned long time) {
        device->setTimestamp(time);
        return *this;
    }
};

#endif // DEVICE_WRAPPER_H
