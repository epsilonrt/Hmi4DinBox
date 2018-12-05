// Hmi4DinBox Class: LCD Helloworld Example
// by epsilonrt <https://github.com/epsilonrt>

// How to control the LCD ?

// Created 18 may 2018

// This example code is in the public domain.

#include <Hmi4DinBox.h>

Hmi4DinBox hmi;

void setup() {

  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, HIGH);  // the led lights to indicate the beginning of the setup ()
  if (!hmi.begin (24)) {

    exit (1); // HMI failed to start !
  }
  digitalWrite (LED_BUILTIN, LOW); // the led goes off to indicate the end of the setup ()
}

void loop() {
  static int counter = 0;

  digitalWrite (LED_BUILTIN, HIGH);  // the led turns on and off at each loop.
  // Write a piece of text on the first line...
  hmi.lcd.setCursor (0, 0); //LINE 1, ADDRESS 0
  hmi.lcd.print ("Hello World");

  // Write the counter on the second line...
  hmi.lcd.setCursor (1, 0);
  hmi.lcd.print (counter / 10, DEC);
  hmi.lcd.write ('.');
  hmi.lcd.print (counter % 10, DEC);
  hmi.lcd.write (' ');
  counter++;
  delay (250);
  digitalWrite (LED_BUILTIN, LOW);
  delay (250);
}
