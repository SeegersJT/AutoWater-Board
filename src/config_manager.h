#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <SPIFFS.h>

extern int LCD_ADDR;
extern int LCD_COLUMNS;
extern int LCD_ROWS;
extern int MOISTURE_SENSOR_PIN_MM01;
extern int RELAY_PIN_R01;
extern int LCD_SDA_PIN;
extern int LCD_SCL_PIN;
extern int MOISTURE_SENSORS_MIN;
extern int MOISTURE_SENSORS_MAX;
extern int ACTIVATE_RELAY_THRESHOLD;
extern unsigned long MOISTURE_SENSORS_CHECK_INTERVAL;
extern unsigned long RELAY_ON_DURATION;

void readConfig();

#endif // CONFIG_MANAGER_H
