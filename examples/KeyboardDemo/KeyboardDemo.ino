// Hmi4DinBox Class: Keyboard Example
// by epsilonrt <https://github.com/epsilonrt>

// How to read the keyboard ?

// Created 18 may 2018

// This example code is in the public domain.

#include <Hmi4DinBox.h>
#include <LCD_ST7032.h>

const int hirqPin = 2;
Hmi4DinBox hmi (hirqPin);

LCD_ST7032 lcd;
int loops = 0;

void setup() {

  // starts the LCD, need to repeat the call in the case of a USB power boot...
  for (int i = 0; i < 2; i++) {
    lcd.begin();  // this function calls Wire.begin(), no need to double the call!
  }
  lcd.setcontrast (24); // contrast value range is 0-63, try 25@5V or 50@3.3V as a starting value
  lcd.cursor();
  lcd.blink();

  while (!hmi.begin()) { // start the HMI by checking that it has worked well...
    lcd.clear();
    lcd.print ("Wait hmi:");
    lcd.print (++loops);
    delay (500);
  }
  lcd.clear();
}

void loop() {
  static int column = 0;
  static int row = 0;

  if (hmi.keyb.available()) { // check if keys are available
    byte key = hmi.keyb.key(); // get the next key

    if (hmi.keyb.released()) { // this key was released ?
    
      lcd.write ('R'); // yes, print R
      hmi.led.clear(LED_GREEN1);
    }
    else {

      lcd.write ('P'); // no, print P
      hmi.led.set(LED_GREEN1);
    }

    lcd.print (key); // print the key
    
    // Move the cursor when you reach the end of the line.
    column += 2;
    if (column >= 16) { // end of line ?
      // yes
      if (++row > 1) { // end of screen ?
        // wait for reading last key and clear the LCD
        delay(500);
        row = 0;
        lcd.clear();
      }
      column = 0;
      lcd.setCursor (row, column);
    }

  }
}

//------------------------------------------------------------------------------
