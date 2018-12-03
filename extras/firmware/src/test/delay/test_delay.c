/**
 * @file test_delay.c
 * @brief Génère un signal rectangulaire de période 100 ms
 * 
 * La configuration du module LED est définie dans board/avrio-board-led.h
 * 
 * Copyright © 2014 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avrio/led.h>
#include <avrio/delay.h>

/* internal public functions ================================================ */
int
main (void) {

  vLedInit ();

  for (;;) {

    delay_ms (50);
    vLedToggle (LED_LED1);
  }
  return 0;
}

/* ========================================================================== */
