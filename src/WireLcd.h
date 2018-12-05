//-------------------------------------------------------------------------------------------
// WireLcd.h - Arduino i2c master library for LCD ST7032
// Olav Kallhovd sept2017
//
// Tested Module    : ERC1602-4, EASTRISING TECHNOLOGY CO,.LTD.
// Drive IC         : ST7032
// INTERFACE        : I2C
// VDD              : 2.7V-5.5V
// Tested with MCU  : Arduino Uno, Attiny85@1mhz(5.0V), Attiny85@8mhz(5.0V) and ESP8266(3.3V)
//-------------------------------------------------------------------------------------------

#ifndef WireLcd_h
#define WireLcd_h

#include <Arduino.h>

#define CONTRAST_MAX                  0x3F //limit range max value (0x00 - 0x3F)
#define CONTRAST_MIN                  0x00 //limit range min value (0x00 - 0x3F)

#define DISPLAY_ON_OFF                0x08 //display on, cursor on, cursor position on
#define DISPLAY_ON_OFF_B              0x01 //cursor position on
#define DISPLAY_ON_OFF_C              0x02 //cursor on
#define DISPLAY_ON_OFF_D              0x04 //display on

class WireLcd: public Print {
  public:
    WireLcd();
    void begin (bool boost = false);
    void clear();
    void home();
    void display();
    void noDisplay();
    void setCursor (uint8_t line, uint8_t pos);
    void cursor();
    void noCursor();
    void blink();
    void noBlink();
    void setcontrast (int val);
    void adjcontrast (int val);
    uint8_t getcontrast();
    virtual size_t write (uint8_t chr);

  protected:
    void Write_Instruction (uint8_t cmd);
    void Write_Data (uint8_t data);
    uint8_t displayOnOffSetting = (DISPLAY_ON_OFF | DISPLAY_ON_OFF_D);
    uint8_t contrast = 0x18;
    bool boost;
};

#endif
