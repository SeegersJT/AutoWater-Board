#ifndef DISPLAY_WRAPPER_H
#define DISPLAY_WRAPPER_H

#include <LiquidCrystal_I2C.h>
#include "config_manager.h"

class DisplayWrapper {
public:
    bool shouldClear;
    int targetColumn;
    int targetRow;
    String message;

    DisplayWrapper(String message);

    DisplayWrapper& clear();
    DisplayWrapper& row(int row);
    DisplayWrapper& top();
    DisplayWrapper& bottom();
    DisplayWrapper& column(int column);
    void print();

    static void init();
    static void clearDisplay();
    static void backlight(bool enableBacklight = true);
    static void checkBacklight();
    static unsigned long lastDisplayTime;
};

#endif // DISPLAY_WRAPPER_H
