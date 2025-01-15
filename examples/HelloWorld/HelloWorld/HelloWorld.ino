// Hmi4DinBox Class: LCD Helloworld Example
// by epsilonrt <https://github.com/epsilonrt>

// How to control the LCD ?

// Created 18 may 2018

// This example code is in the public domain.
#ifdef __unix__
#include <Piduino.h>  // All the magic is here ;-)
#else
// Defines the serial port as the console on the Arduino platform
#define Console Serial
#endif

#include <Hmi4DinBox.h>

const int lcdContrast = 34; // 24 if you use a 5V power supply, 34 if you use a 3.3V power supply
const bool lcdBoost = true; // false if you use a 5V power supply, true if you use a 3.3V power supply

Hmi4DinBox hmi;

void setup() {

  Console.begin (115200);
  Wire.begin(); // change this if you use another I2C port or if you use non standard I2C pins
  if (!hmi.begin (lcdContrast, lcdBoost)) {

    Console.println("hmi.begin() failed !");
    exit (1); // HMI failed to start !
  }
}

void loop() {
  static int counter = 0;

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
  delay (500);
}
