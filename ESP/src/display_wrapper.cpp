#include "display_wrapper.h"
#include "globals.h"

LiquidCrystal_I2C lcd_display(config["LCD_ADDR"], config["LCD_COLUMNS"], config["LCD_ROWS"]);

unsigned long display::lastDisplayTime = 0;

display::display(String message)
    : message(message), shouldClear(false), targetColumn(0), targetRow(0) {}

display& display::clear() {
    this->shouldClear = true;
    return *this;
}

display& display::row(int row) {
    this->targetRow = row;
    return *this;
}

display& display::top() {
    this->targetRow = 0;
    return *this;
}

display& display::bottom() {
    this->targetRow = 1;
    return *this;
}

display& display::column(int column) {
    this->targetColumn = column;
    return *this;
}

void display::print() {
    if (this->shouldClear) lcd_display.clear();

    lcd_display.setCursor(this->targetColumn, this->targetRow);
    lcd_display.print(this->message);

    Serial.println("[" + this->message + "]");

    display::lastDisplayTime = millis();
    display::backlight();
}

void display::init() {
    lcd_display.init();
    display::backlight();
}

void display::clearDisplay() {
    lcd_display.clear();
}

void display::backlight(bool enableBacklight) {
    if (enableBacklight) {
        lcd_display.backlight();
    } else {
        lcd_display.noBacklight();
    }
}

void display::checkBacklight() {
    unsigned long currentTime = millis();
    if (currentTime - lastDisplayTime > 15000) {
        display::backlight(false);
    }
}
