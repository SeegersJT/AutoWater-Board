#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include "config.h"
#include "devices.h"

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);

RelayDevice relay(RELAY_PIN_R01, "RELAY 01");
MoistureDevice moistureMeter(MOISTURE_METER_PIN_MM01, "MOISTURE METER 01");

// =========================================================
// START UP
// =========================================================
void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
}

// =========================================================
// OUTPUTS - Serial print terminal data.
// =========================================================
void outputMoistureMeterData(const MoistureDevice& moistureDevice) {
  int moisturePercentage = moistureDevice.getMoisturePercentage();

  Serial.print("MOISTURE METER - ");
  Serial.print(moistureDevice.name);
  Serial.print(" : ");
  Serial.print(moisturePercentage);
  Serial.println("%");
}

void outputRelayState(const RelayDevice& relayDevice) {
  Serial.print("RELAY - ");
  Serial.print(relayDevice.name);
  Serial.print(" : ");
  Serial.println(relayDevice.getState());
}

// =========================================================
// LCD DISPLAY - Show the moisture meter percentage and date/time.
// =========================================================
void displayMoistureOnLCD(const MoistureDevice& moistureDevice) {
  // Get current time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture Meter Data - ");
  lcd.print(timeString);
  
  lcd.setCursor(0, 1);
  lcd.print(moistureDevice.name);
  lcd.print(" : ");
  lcd.print(moistureDevice.getMoisturePercentage());
  lcd.print("%");
}

// =========================================================
// CONNECTORS - Logic to control relay based on moisture meter
// =========================================================
void connectRelayToMoistureMeter(RelayDevice& relayDevice, MoistureDevice& moistureDevice) {
  moistureDevice.updateState();

  outputMoistureMeterData(moistureDevice);

  if (moistureDevice.getMoisturePercentage() < MOISTURE_METERS_THRESHOLD) {
    relayDevice.setState("ON");
  } else {
    relayDevice.setState("OFF");
  }

  outputRelayState(relayDevice);
  displayMoistureOnLCD(moistureDevice);
}

// =========================================================
// SETUP - Executed code at the start of the run
// =========================================================
void setup() {
  Serial.begin(115200);

  connectToWiFi();

  relay.setup();
  moistureMeter.setup();

  lcd.begin();
  lcd.backlight();
}

// =========================================================
// LOOP - Executed code in a loop
// =========================================================
void loop() {
  connectRelayToMoistureMeter(relay, moistureMeter);

  delay(1000);
}
