// Hmi4DinBox Class: LCD Backlight Example
// by epsilonrt <https://github.com/epsilonrt>

// How to control the backlight ?

// Created 18 may 2018

// This example code is in the public domain.

#include <Hmi4DinBox.h>
#include <LCD_ST7032.h>

const int hirqPin = 2;
Hmi4DinBox hmi (hirqPin);

LCD_ST7032 lcd;
int loops = 0;
byte bl;

void setup() {

  // starts the LCD, need to repeat the call in the case of a USB power boot...
  for (int i = 0; i < 2; i++) {
    lcd.begin();  // this function calls Wire.begin(), no need to double the call!
  }
  lcd.setcontrast (24); // contrast value range is 0-63, try 25@5V or 50@3.3V as a starting value

  while (!hmi.begin()) { // start the HMI by checking that it has worked well...
    lcd.clear();
    lcd.print ("Wait hmi:");
    lcd.print (++loops);
    delay (500);
  }

  // read value stored in EEPROM.
  bl = hmi.backlight.read();

  // print this value
  lcd.clear();
  lcd.print ("Backlight");
  lcd.setCursor (0, 10);
  lcd.print (bl);
  lcd.print ("  ");
}

void loop() {

  if (hmi.keyb.available()) { // check if keys are available

    if (hmi.keyb.pressed()) {
      byte key = hmi.keyb.key(); // get the next key

      if ( (key == KUP) || (key == KDOWN)) { // if key UP or DOWN
        
        if (key == KUP) {
          
          bl += 8; // UP for increase
        }
        else {

          bl -= 8;  // DOWN for decrease
        }
        
        // change value and print
        hmi.backlight.write (bl);
        lcd.setCursor (0, 10);
        lcd.print (bl);
        lcd.print ("  ");
        hmi.led.toggle(LED_GREEN1);
      }
    }
  }
}

//------------------------------------------------------------------------------
