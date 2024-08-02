#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "globals.h"

class WiFiManager {
public:
  static void connectToWiFi(int maxRetries = 3, unsigned long retryDelay = 500) {
    display("Connecting WiFi").clear().print();

    // TODO: REPLACE WITH CONFIG VALUES
    const char* WIFI_SSID = "";
    const char* WIFI_PASSWORD = "";

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

  static void setHostname(char* hostname = (char*)"LEEF") {
    WiFi.setHostname(hostname);
  }

  static WiFiServer server;

  static void enableAccessPoint(char* ssid = (char*)"L.E.E.F.") {
    // Set web server port number to 80
    WiFi.mode(WIFI_AP_STA);

    server = WiFiServer(80);

    IPAddress local_ip(10,0,0,1);
    IPAddress gateway(10,0,0,1);
    IPAddress subnet(255,255,255,0);

    Serial.print("Starting LEEF Access Point");
    WiFi.softAPConfig(local_ip, gateway, subnet);

    WiFi.softAP(ssid, NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP Address: ");
    Serial.println(IP);

    server.begin();
  }

  static void accessPointListen() {
    WiFiClient client = server.available();   // Listen for incoming clients

    if(!client) return;

    // Variable to store the HTTP request
    String header;

    Serial.println("New WiFi Client.");
    String currentLine = "";

    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if(c == '\n') {
          if(currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("COntent-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println("<DOCTYPE html><head><title>L.E.E.F.</title></head><body>");
            client.println("<h1>Hello, L.E.E.F!</h1>");
            client.println("</body></html>");
            client.println();

            break;
          } else {
            currentLine = "";
          }
        } else if(c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
};

#endif // WIFI_MANAGER_H
