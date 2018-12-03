// Hmi4DinBox Class: LED Example
// by epsilonrt <https://github.com/epsilonrt>

// How to control the leds ?

// Created 18 may 2018

// This example code is in the public domain.

#include <Hmi4DinBox.h>
#include <LCD_ST7032.h>

Hmi4DinBox hmi;
LCD_ST7032 lcd;
int loops = 0;
void printLed (int led);

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
  lcd.print ("12345");
}

void loop() {

  // turn on all leds
  hmi.led.writeAll();
  // Pause
  delay (2000);

  // turn off all leds
  hmi.led.writeAll (0);
  // Pause
  delay (2000);

  // turn on the LEDs one by one.
  hmi.led.set (LED1);
  delay (500);
  hmi.led.set (LED2);
  delay (500);
  hmi.led.set (LED3);
  delay (500);
  hmi.led.set (LED4);
  delay (500);
  hmi.led.set (LED5);
  delay (500);
  // Pause
  delay (2000);

  // turn off the LEDs one by one with a for loop
  for (int led = LED1; led <= LED5; led++) {
    hmi.led.clear (led);
    delay (500);
  }
  // Pause
  delay (2000);

  // turn on the LEDs one by with a for loop, using toggle and get value.
  for (int led = LED1; led <= LED5; led++) {
    hmi.led.toggle (led);
    printLed (led);
    delay (500);
  }
  // Pause
  delay (2000);

  // turn off the LEDs one by with a for loop, using toggle() and get()
  for (int led = LED1; led <= LED5; led++) {
    hmi.led.toggle (led);
    printLed (led);
    delay (500);
  }
  // Pause
  delay (2000);
}

void printLed (int led) {
  lcd.setCursor (1, led);
  if (hmi.led.get (led)) {
    lcd.write ('O');
  }
  else {

    lcd.write ('_');
  }
}

//------------------------------------------------------------------------------
