/*
   Copyright (c) 2018-2023, Pascal JEAN aka epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#include "Hmi4DinBox.h"
#include "version.h"

// -----------------------------------------------------------------------------
//
//                            Hmi4DinBox class
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
Hmi4DinBox::Hmi4DinBox (int hirqPin, byte slaveAddress) :
  WireSlave (slaveAddress),
  led (this, LED_REG, LED_NB),
  keyb (this, KEY_REG, hirqPin),
  backlight (this, BACKLIGHT_REG) {
}

// -----------------------------------------------------------------------------
bool Hmi4DinBox::begin (int lcdContrast, bool lcdBoost) {
  int loops = 0;
  const int loopTimeout = 3000 / 500; // timeout 3s
  bool ledStarted = false;
  bool keybStarted = false;
  bool backlightStarted = false;

  // starts the LCD, need to repeat the call in the case of a USB power boot...
  for (int i = 0; i < 2; i++) {

    this->lcd.begin (lcdBoost); // this function calls Wire.begin(), no need to double the call!
  }
  
  this->lcd.setcontrast (lcdContrast);
  this->lcd.clear();
  //                0123456789012345
  this->lcd.print ("Hmi4DinBox ");
  this->lcd.print (VERSION_SHORT);
  this->lcd.setCursor (1, 0);

  while (1) { // start the HMI by checking that it has worked well...

    if (!ledStarted) {

      ledStarted = this->led.begin();
    }

    if (!keybStarted) {

      keybStarted = this->keyb.begin();
    }

    if (!backlightStarted) {

      backlightStarted = this->backlight.begin();
    }

    if ( (ledStarted && keybStarted && backlightStarted) || (loops > loopTimeout)) {

      this->lcd.clear();
      break;
    }

    this->lcd.write ('.');
    delay (500);
    ++loops;
  }

  return  ledStarted && keybStarted && backlightStarted;
}

/* ========================================================================== */
