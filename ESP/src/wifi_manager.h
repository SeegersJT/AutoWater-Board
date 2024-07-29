#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "secrets.h"
#include "display_wrapper.h"

using display = DisplayWrapper;

class WiFiManager {
public:
    static void connectToWiFi(int maxRetries = 10, unsigned long retryDelay = 1000) {
        display("Connecting WiFi").clear().print();
        
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        int retries = 0;
        while (WiFi.status() != WL_CONNECTED && retries < maxRetries) {
            delay(retryDelay);
            retries++;
            display("Connecting... " + String(retries) + "/" + String(maxRetries)).bottom().print();
        }

        if (WiFi.status() == WL_CONNECTED) {
            display("Connected").clear().print();
            display(WiFi.localIP().toString()).bottom().print();
        } else {
            display("Failed to connect").clear().print();
        }

        delay(2000);
    }
};

#endif // WIFI_MANAGER_H
