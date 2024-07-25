#include <LiquidCrystal_I2C.h>
#include "config.h"
#include <string>

LiquidCrystal_I2C lcd_display(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);

class DisplayWrapper {
  public:
    bool shouldClear = false;
    int targetColumn = 0;
    int targetRow = 0;
    String message;

    DisplayWrapper(String message) {
      this->message = message;
    }

    DisplayWrapper& clear() {
      this->shouldClear = true;
      return *this;
    }

    DisplayWrapper& row(int row) {
      this->targetRow = row;
      return *this;
    }

    DisplayWrapper& top() {
      this->targetRow = 0;
      return *this;
    }

    DisplayWrapper& bottom() {
      this->targetRow = 1;
      return *this;
    }

    DisplayWrapper& column(int column) {
      this->targetColumn = column;
      return *this;
    }

    void print() {
      // do printing stuff (get lots of ink ready)
      if(this->shouldClear) lcd_display.clear();

      lcd_display.setCursor(this->targetColumn, this->targetRow);

      lcd_display.print(this->message);
    }

    static void init() {
      lcd_display.init();
      DisplayWrapper::backlight();
    }

    static void clearDisplay() {
      lcd_display.clear();
    }

    static void backlight(bool enableBacklight = true) {
      if(enableBacklight) {
        lcd_display.backlight();
      } else {
        lcd_display.noBacklight();
      }
    }
};
