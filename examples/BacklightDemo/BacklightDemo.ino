// Hmi4DinBox Class: LCD Backlight Example
// by epsilonrt <https://github.com/epsilonrt>

// How to control the backlight ?

// Created 18 may 2018

// This example code is in the public domain.

#include <Hmi4DinBox.h>

const int hirqPin = 7;
Hmi4DinBox hmi (hirqPin);

byte bl;
void printBacklight ();

void setup() {

  if (!hmi.begin (24)) {

    exit (1); // HMI failed to start !
  }

  // read value stored in EEPROM.
  bl = hmi.backlight.read();

  // print this value
  hmi.lcd.clear();
  hmi.lcd.print ("Backlight");
  printBacklight();
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
        printBacklight();
        hmi.led.toggle (LED_GREEN1);
      }
    }
  }
}

void printBacklight () {

  hmi.lcd.setCursor (0, 10);
  hmi.lcd.print (bl);
  hmi.lcd.print ("  ");
}

//------------------------------------------------------------------------------
