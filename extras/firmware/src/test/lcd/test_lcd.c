/**
 * @file test_lcd.c
 * @brief Test afficheur LCD
 *
 * Affiche un message et un compte à rebours puis attend des caractères sur
 * la liaison série et les affichent sur le LCD:
 * - Un appui sur CTRL + C efface le LCD.
 * - Un appui sur Esc relance le test
 *
 * La configuration du module LCD est définie dans board/avrio-board-lcd.h
 * La configuration du module LCD_BACKLIGHT est définie dans board/avrio-board-lcdbacklight.h
 *
 * Copyright © 2014 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <ctype.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/lcd.h>


/* main ===================================================================== */
int
main (void) {
  int i;

  vLedInit();

  /*
   * Init LCD avec réglage rétro-éclairage et contraste
   * Dans le cas de la carte XNODE, l'afficheur est connecté par I2C
   */
  iLcdInit();
  stdout = &xLcd;

  /*
   * La liaison série L0 est définie comme entrée et sortie standard (printf, scanf ...)
   */
  // sei();

  for (;;) {

    // Message et compte à rebours
    vLcdClear ();
    printf ("Hello World!\n");
    for (i = 9; i >= 0; i--) {

      printf ("%d", i);
      delay_ms (500);
    }
    vLcdGotoXY (13, 0);
    printf ("Go!");
    delay_ms (5000);
  }
  return 0;
}
/* ========================================================================== */
