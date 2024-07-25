#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include "config.h"
#include "devices.h"

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);

RelayDevice relay(RELAY_PIN_R01, "RELAY 01");
MoistureDevice moistureMeter(MOISTURE_METER_PIN_MM01, "MOISTURE METER 01");

void displayMessage(const char* message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
}

// =========================================================
// START UP
// =========================================================

void connectToWiFi() {
  displayMessage("Connecting to WiFi...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);

    Serial.print('.');

    lcd.setCursor(0, 1);
    lcd.print("Connecting...");
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected to WiFi");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  delay(2000);
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
// LCD DISPLAY - Show the moisture meter percentage and date/time.
// =========================================================

void displayMoistureMeterData(const MoistureDevice& moistureDevice) {
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
  lcd.print(moistureDevice.name);
  lcd.print(" : ");
  lcd.print(moistureDevice.getMoisturePercentage());
  lcd.print("%");

  Serial.print("Connecting to WiFi ..");
}

// =========================================================
// CONNECTORS - Logic to control relay based on moisture meter
// =========================================================

void connectRelayToMoistureMeter(RelayDevice& relayDevice, MoistureDevice& moistureDevice) {
  moistureDevice.updateState();

  if (moistureDevice.getMoisturePercentage() < MOISTURE_METERS_THRESHOLD) {
    relayDevice.setState("ON");
  } else {
    relayDevice.setState("OFF");
  }

  outputMoistureMeterData(moistureDevice);
  outputRelayState(relayDevice);

  displayMoistureMeterData(moistureDevice);
}

// =========================================================
// SETUP - Executed code at the start of the run
// =========================================================

void setup() {
  Serial.begin(9600);

  // lcd.begin(LCD_COLUMNS, LCD_ROWS);
  // lcd.backlight();

  // displayMessage("Starting...");
  // delay(5000);

  // connectToWiFi();

  // relay.setup();
  // moistureMeter.setup();
}

// =========================================================
// LOOP - Executed code in a loop
// =========================================================

void loop() {
  Serial.print("MOISTURE METER - ");
  Serial.print(" : ");
  Serial.println("%");

  delay(1000);
}
