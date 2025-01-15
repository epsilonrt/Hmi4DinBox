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

#include <WireLcd.h>

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) \
    || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) \
    || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) \
    || defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny4313__) \
    || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__) \
    || defined( __AVR_ATtiny261__ ) || defined( __AVR_ATtiny461__ ) \
    || defined( __AVR_ATtiny861__ ) || defined(__AVR_ATtiny1634__)
#include <USIWire.h>
#else
#include <Wire.h>
#endif


#define Write_Address                 0x3E //i2c address
#define CNTRBIT                       0x00 //followed by command bytes
#define CNTRBIT_CO                    0x80 //followed by 1 command byte
#define CNTRBIT_RS                    0x40 //after last control byte, followed by DDRAM data byte(s)
#define CLEAR_DISPLAY                 0x01 //Clear display
#define RETURN_HOME                   0x02 //Cursor home to 00H
#define ENTRY_MODE_SET                0x04 //Sets cursor move direction and specifies display shift.
#define FUNCTION_SET                  0x20 //DL: interface data is 8/4 bits, N: number of line is 2/1 DH: double height font, IS: instruction table select
#define SET_DDRAM_ADDRESS             0x80 //Set DDRAM address in address counter
#define CURSOR_OR_DISPLAY_SHIFT       0x10 //Set cursor moving and display shift control bit, and the direction without changing DDRAM data.
#define SET_CGRAM_ADDRESS             0x40 //Set CGRAM address in address counter
#define INTERNAL_OSC_FREQ             0x10 //BS=1:1/4 bias, BS=0:1/5 bias, F2~0: adjust internal OSC frequency for FR frequency.
#define POWER_ICON_BOST_CONTR         0x50 //Ion: ICON display on/off, Bon: set booster circuit on/off, C5,C4: Contrast set
#define FOLLOWER_CONTROL              0x60 //Fon: set follower circuit on/off, Rab2~0: select follower amplified ratio.
#define CONTRAST_SET                  0x70 //C0-C3: Contrast set 
#define LINE_1_ADR                    0x80
#define LINE_2_ADR                    0xC0

// Various flags and masks
#define ENTRY_MODE_SET_S              0x01 //S: Shift of entire display, see data sheet
#define ENTRY_MODE_SET_ID             0x02 //I/D : Increment / decrement of DDRAM address (cursor or blink), see  data sheet
#define FUNCTION_SET_IS               0x01 //IS: instruction table select
#define FUNCTION_SET_DH               0x04 //DH: double height font
#define FUNCTION_SET_N                0x08 //N: number of line is 2/1
#define FUNCTION_SET_DL               0x10 //DL: interface data is 8/4 bits
#define CURSOR_OR_DISPLAY_SHIFT_RL    0x04 //
#define CURSOR_OR_DISPLAY_SHIFT_SC    0x08 //
#define INTERNAL_OSC_FREQ_F0          0x01 //F2~0: adjust internal OSC frequency for FR frequency.
#define INTERNAL_OSC_FREQ_F1          0x02 //F2~0: adjust internal OSC frequency for FR frequency.
#define INTERNAL_OSC_FREQ_F2          0x04 //F2~0: adjust internal OSC frequency for FR frequency.
#define INTERNAL_OSC_FREQ_BS          0x08 //BS=1:1/4 bias (BS=0:1/5 bias)
#define POWER_ICON_BOST_CONTR_Bon     0x04 //Bon: set booster circuit on/off
#define POWER_ICON_BOST_CONTR_Ion     0x08 //Ion: ICON display on/off
#define FOLLOWER_CONTROL_Rab0         0x01 //Rab2~0: select follower amplified ratio
#define FOLLOWER_CONTROL_Rab1         0x02 //Rab2~0: select follower amplified ratio
#define FOLLOWER_CONTROL_Rab2         0x04 //Rab2~0: select follower amplified ratio
#define FOLLOWER_CONTROL_Fon          0x08 //Fon: set follower circuit on/off

#define WRITE_DELAY_MS                  30 //see data sheet
#define HOME_CLEAR_DELAY_MS       1200 //see data sheet

WireLcd::WireLcd() {
}

void WireLcd::begin (bool b) {
  boost = b;
  delay (100);
  Write_Instruction (FUNCTION_SET | FUNCTION_SET_DL | FUNCTION_SET_N | FUNCTION_SET_IS);
  Write_Instruction (INTERNAL_OSC_FREQ | INTERNAL_OSC_FREQ_BS | INTERNAL_OSC_FREQ_F2);
  Write_Instruction (POWER_ICON_BOST_CONTR | POWER_ICON_BOST_CONTR_Ion);
  setcontrast (contrast);
  Write_Instruction (FOLLOWER_CONTROL | FOLLOWER_CONTROL_Fon | FOLLOWER_CONTROL_Rab2);
  delay (300);
  Write_Instruction (displayOnOffSetting);
  Write_Instruction (ENTRY_MODE_SET | ENTRY_MODE_SET_ID);
  this->clear();
  this->home();
}

void WireLcd::Write_Instruction (uint8_t cmd) {
  Wire.beginTransmission (Write_Address);
  Wire.write (CNTRBIT_CO);
  Wire.write (cmd);
  Wire.endTransmission();
  delayMicroseconds (WRITE_DELAY_MS);
}

void WireLcd::Write_Data (uint8_t data) {
  Wire.beginTransmission (Write_Address);
  Wire.write (CNTRBIT_RS);
  Wire.write (data);
  Wire.endTransmission();
  delayMicroseconds (WRITE_DELAY_MS);
}

size_t WireLcd::write (uint8_t chr) {
  this->Write_Data (chr);
  return 1;
}

void WireLcd::clear() { //clear display
  this->Write_Instruction (CLEAR_DISPLAY);
  delayMicroseconds (HOME_CLEAR_DELAY_MS);
}

void WireLcd::home() { //return to first line address 0
  this->Write_Instruction (RETURN_HOME);
  delayMicroseconds (HOME_CLEAR_DELAY_MS);
}

void WireLcd::setCursor (uint8_t line, uint8_t pos) {
  uint8_t p;
  if (pos > 15) {
    pos = 0;
  }
  if (line == 0) {
    p = LINE_1_ADR + pos;
  }
  else {
    p = LINE_2_ADR + pos;
  }
  Write_Instruction (SET_DDRAM_ADDRESS | p);
}

void WireLcd::display() { //turn on display
  displayOnOffSetting |= DISPLAY_ON_OFF_D;
  Write_Instruction (displayOnOffSetting);
}

void WireLcd::noDisplay() { //turn off display
  displayOnOffSetting &= ~DISPLAY_ON_OFF_D;
  Write_Instruction (displayOnOffSetting);
}

void WireLcd::cursor() { //display underline cursor
  displayOnOffSetting |= DISPLAY_ON_OFF_C;
  Write_Instruction (displayOnOffSetting);
}

void WireLcd::noCursor() { //stop display underline cursor
  displayOnOffSetting &= ~DISPLAY_ON_OFF_C;
  Write_Instruction (displayOnOffSetting);
}

void WireLcd::blink() { //cursor block blink
  displayOnOffSetting |= DISPLAY_ON_OFF_B;
  Write_Instruction (displayOnOffSetting);
}

void WireLcd::noBlink() { //stop cursor block blink
  displayOnOffSetting &= ~DISPLAY_ON_OFF_B;
  Write_Instruction (displayOnOffSetting);
}

void WireLcd::setcontrast (int val) {
  if (val > CONTRAST_MAX) {
    val = CONTRAST_MIN;
  }
  else if (val < CONTRAST_MIN) {
    val = CONTRAST_MAX;
  }
  Write_Instruction (CONTRAST_SET | (val & B00001111));
  Write_Instruction ( (val >> 4) | POWER_ICON_BOST_CONTR | (boost ? POWER_ICON_BOST_CONTR_Bon : 0));
  contrast = val;
}

void WireLcd::adjcontrast (int val) {
  setcontrast (val + contrast);
}

uint8_t WireLcd::getcontrast() {
  return contrast;
}
