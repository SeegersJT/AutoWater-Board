#include "display_wrapper.h"

LiquidCrystal_I2C lcd_display(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);

unsigned long DisplayWrapper::lastDisplayTime = 0;

DisplayWrapper::DisplayWrapper(String message)
    : message(message), shouldClear(false), targetColumn(0), targetRow(0) {}

DisplayWrapper& DisplayWrapper::clear() {
    this->shouldClear = true;
    return *this;
}

DisplayWrapper& DisplayWrapper::row(int row) {
    this->targetRow = row;
    return *this;
}

DisplayWrapper& DisplayWrapper::top() {
    this->targetRow = 0;
    return *this;
}

DisplayWrapper& DisplayWrapper::bottom() {
    this->targetRow = 1;
    return *this;
}

DisplayWrapper& DisplayWrapper::column(int column) {
    this->targetColumn = column;
    return *this;
}

void DisplayWrapper::print() {
    if (this->shouldClear) lcd_display.clear();

    lcd_display.setCursor(this->targetColumn, this->targetRow);
    lcd_display.print(this->message);

    DisplayWrapper::lastDisplayTime = millis();
    DisplayWrapper::backlight();
}

void DisplayWrapper::init() {
    lcd_display.init();
    DisplayWrapper::backlight();
}

void DisplayWrapper::clearDisplay() {
    lcd_display.clear();
}

void DisplayWrapper::backlight(bool enableBacklight) {
    if (enableBacklight) {
        lcd_display.backlight();
    } else {
        lcd_display.noBacklight();
    }
}

void DisplayWrapper::checkBacklight() {
    unsigned long currentTime = millis();
    if (currentTime - lastDisplayTime > 15000) {
        DisplayWrapper::backlight(false);
    }
}
