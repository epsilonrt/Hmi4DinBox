// Hmi4DinBox Class: Keyboard Example
// by epsilonrt <https://github.com/epsilonrt>

// How to read the keyboard ?

// Created 18 may 2018

// This example code is in the public domain.
#ifdef __unix__
#include <Piduino.h>  // All the magic is here ;-)
#else
// Defines the serial port as the console on the Arduino platform
#define Console Serial
#endif

#include <Hmi4DinBox.h>

const int hirqPin = 7;
Hmi4DinBox hmi (hirqPin);

void setup() {

  Console.begin (115200);
  if (!hmi.begin (24, false)) {

    Console.println("hmi.begin() failed !");
    exit (1); // HMI failed to start !
  }
  
  hmi.lcd.clear();
  hmi.lcd.cursor();
  hmi.lcd.blink();
}

void loop() {
  static int column = 0;
  static int row = 0;

  if (hmi.keyb.available()) { // check if keys are available
    byte key = hmi.keyb.key(); // get the next key

    if (hmi.keyb.released()) { // this key was released ?
    
      hmi.lcd.write ('R'); // yes, print R
      hmi.led.clear(LED_GREEN1);
    }
    else {

      hmi.lcd.write ('P'); // no, print P
      hmi.led.set(LED_GREEN1);
    }

    hmi.lcd.print (key); // print the key
    
    // Move the cursor when you reach the end of the line.
    column += 2;
    if (column >= 16) { // end of line ?
      // yes
      if (++row > 1) { // end of screen ?
        // wait for reading last key and clear the LCD
        delay(500);
        row = 0;
        hmi.lcd.clear();
      }
      column = 0;
      hmi.lcd.setCursor (row, column);
    }

  }
}
