#ifndef CONFIG_H
#define CONFIG_H

// LCD 16x2 (I2C)
const int LCD_ADDR = 0x27;
const int LCD_COLUMNS = 20;
const int LCD_ROWS = 4;

// PINS
const int MOISTURE_METER_PIN_MM01 = 36;
const int RELAY_PIN_R01 = 23;
const int LCD_SDA_PIN = 21;
const int LCD_SCL_PIN = 22;

// CONSTANTS
const int MOISTURE_METERS_MIN = 1200;
const int MOISTURE_METERS_MAX = 4095;
const int MOISTURE_METERS_THRESHOLD = 50;

// DISPLAY POSITIONS
const int DISPLAY_TOP = 0;
const int DISPLAY_BOTTOM = 1;

#endif // CONFIG_H