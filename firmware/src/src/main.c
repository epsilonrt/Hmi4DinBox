/**
 * @file main.c
 * @brief Interface Homme-Machine Projet Toueris 2 - Programme principal
 */
#include "hmi.h"

//xMutex xHmiMutex = MUTEX_INITIALIZER;

/* main ===================================================================== */
int
main (void) {
  
  vHmiInit();

  for (;;) {

    vHmiLoop();
  }
  return 0;
}

/* ========================================================================== */
