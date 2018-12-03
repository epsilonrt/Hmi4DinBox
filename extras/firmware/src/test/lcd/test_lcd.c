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
#include <avrio/twi.h>
#include <avrio/serial.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 38400

/* main ===================================================================== */
int
main (void) {
  int i;

  vLedInit();

  /*
   * Init du bus I2C en mode maître à 400 kHz utilisé par le LCD
   */
  vTwiInit ();
  eTwiSetSpeed (400);

  /*
   * Init LCD avec réglage rétro-éclairage et contraste
   * Dans le cas de la carte XNODE, l'afficheur est connecté par I2C
   */
  iLcdInit();
  ucLcdBacklightSet (32); // 0 à 63
  ucLcdContrastSet (16);  // 0 à 31

  /*
   * La liaison série L0 est définie comme entrée et sortie standard (printf, scanf ...)
   */
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW + SERIAL_NOBLOCK);
  stdout = &xSerialPort;
  stdin = &xSerialPort;
  sei();

  for (;;) {

    printf ("Test LCD\nCTRL+C:\tClear\nEsc:\tReset\n");

    // Message et compte à rebours
    vLcdClear ();
    fprintf (&xLcd, "Hello World!\n");
    for (i = 9; i >= 0; i--) {

      fprintf (&xLcd, "%d", i);
      delay_ms (500);
    }
    vLcdGotoXY (13, 0);
    fprintf (&xLcd, "Go!");

    // Affiche les caractères reçus de la liaison série sur le LCD
    do {
      i = getchar();
      switch (i) {
        case 0x03:    // CTRL + C
          printf ("\tClear\n");
          vLcdClear();
          break;
        case EOF:   // Pas de caractère reçu
        case 0x1B:  // Esc
          break;
        default:
          vLedToggle (LED_LED1);
          putchar (i);
          fputc (i, &xLcd);
          break;
      }
    }
    while (i != 0x1B);
  }
  return 0;
}
/* ========================================================================== */
