/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id: test_lcd_backlight.c 4 2013-01-28 09:43:38Z pjean $ */
/* ========================================================================== */
#include <avrio/lcd.h>
#include <avrio/delay.h>
#include <avrio/button.h>
#include <stdio.h>
#include "backlight.h"
/* main ===================================================================== */
int
main (void) {
  uint8_t ucBacklight = 128;
  uint8_t ucLast = 0;
  xButMask xButton;

  vButInit();
  iLcdInit();
  vHmiBacklightSet (ucBacklight);
  stdout = &xLcd;

  for (;;) {

    if (ucBacklight != ucLast) {

      vHmiBacklightSet (ucBacklight);
      ucLast = ucBacklight;
      vLcdClearLine (0);
      printf ("BL=%d", ucBacklight);
    }
    xButton = xButGet (BUTTON_ALL_BUTTONS);

    if (xButton) {

      if (xButton & BUTTON_BUTTON1) {

        ucBacklight += 8;
      }
      if (xButton & BUTTON_BUTTON2) {

        ucBacklight -= 8;
      }
      while (xButGet (BUTTON_BUTTON1 | BUTTON_BUTTON2))
        ;
    }
  }
  return 0;
}

/* ========================================================================== */
