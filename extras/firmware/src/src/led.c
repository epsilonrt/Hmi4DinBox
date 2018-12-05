/**
 * @file button.c
 * @brief Interface Homme-Machine Projet Toueris 2 - Partie bouton poussoir
 */
#include <Hmi4DinBox.h>
#include "led.h"
#include <avrio/led.h>
#include "config.h" // toujours en dernier

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void vHmiLedInit (void) {
  
  vLedInit();
}

// -----------------------------------------------------------------------------
void
vHmiLedToggleAll (uint8_t value) {

  for (uint8_t led = 0; led < LED_NB; led++) {
    if (_BV (led) & value) {
      vLedToggle (xLedGetMask (led));
    }
  }
}

// -----------------------------------------------------------------------------
void vHmiLedSet (uint8_t led) {
  
  vLedSet (xLedGetMask (led));
}

// -----------------------------------------------------------------------------
void vHmiLedClear (uint8_t led) {
  
  vLedClear (xLedGetMask (led));
}

// -----------------------------------------------------------------------------
void vHmiLedToggle (uint8_t led) {
  
  vLedToggle (xLedGetMask (led));
}

/* ========================================================================== */
