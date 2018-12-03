/**
 * @file test_serial.c
 * @brief Test liaison série asynchrone (L0 hardware)
 * 
 * Affiche un message sur la liaison série puis attend la réception d'un
 * caractère qui est renvoyé pour affichage.
 *
 * La configuration du module SERIAL est définie dans board/avrio-board-serial.h
 * 
 * Copyright © 2014 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avrio/led.h>
#include <avrio/serial.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 38400

/* main ===================================================================== */
int
main (void) {
  int c;

  vLedInit();
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW + SERIAL_NOBLOCK);
  
  // Les fonctions de stdio.h utiliseront le port série en entrée et sortie
  stdout = &xSerialPort;
  stdin = &xSerialPort;
  sei();

  for (;;) {
    // Affiche le message
    printf ("Test d'utilisation du module serial\nAppuyer sur une touche:\n");
    do {
      // Renvoie chaque caractère reçu
      c = getchar();

      if (c != EOF) {
        // Inverse l'état de la led à chaque réception
        vLedToggle (LED_LED1);
        putchar(c);
      }
    } while (c != '\n');
  }
  return 0;
}
/* ========================================================================== */
