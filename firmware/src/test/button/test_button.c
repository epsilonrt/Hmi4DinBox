#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>

/* internal public functions ================================================ */
int
main (void) {
  uint8_t ucBit;

  vLedInit ();
  vButInit ();

  for (;;) {

    for (ucBit = 0; ucBit < BUTTON_QUANTITY; ucBit++) {

      if (xButGet (xButGetMask (ucBit))) {
        uint8_t ucCount = (ucBit + 1) * 2;
        
        while (ucCount--) {
          // La LED clignote une fois pour le bouton 1, 2 fois pour le 2 ....
          vLedToggle (LED_LED1);
          delay_ms (500);
        }
      }
    }
  }
  return 0;
}
/* ========================================================================== */
