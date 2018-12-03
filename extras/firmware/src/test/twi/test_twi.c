#include <avrio/twi.h>
#include <avrio/button.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <string.h>


/* constants ================================================================ */
// Caractéristiques du circuit émulé
#define SLAVE_ADDR  0xA0  // émule un PCF8583
#define RAM_BASE    0x00  // Adresse du premier octet de RAM
#define RAM_SIZE    0x100 // Nombre d'octet de RAM
#define RAM_END     (RAM_BASE + RAM_SIZE - 1)

/* private variables ======================================================== */
static uint8_t ucRamCounter;
static uint8_t ucRamBuffer[RAM_SIZE];

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
prvvCheckError (eTwiStatus eError) {

  if (eError < TWI_SUCCESS) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (500);
    }
  }
}

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vTwiInit ();
  vTwiSetDeviceAddress (0xA0);

  for (;;) {

  }
  return 0;
}

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
// Nous sommes adressé comme esclave en écriture, nous avons reçu une trame
// Attention ! Fonction exécutée sous interruption
eTwiStatus
eTwiSlaveRxCB (xQueue * pxRxPayload, eTwiStatus eStatus) {
  eTwiStatus eError = TWI_SUCCESS;

  switch (eStatus) {

    case TWI_STATUS_GCALL:
      // Ecriture en appel général non implémenté !
      eError = TWI_ERROR_ADDR_NACK;
      break;

    case TWI_STATUS_LCALL:
      if (xQueueLength (pxRxPayload) == 1) {

        ucRamCounter = ucQueuePull (pxRxPayload);

      }
      else if (xQueueLength (pxRxPayload) > 1) {

        ucRamCounter = ucQueuePull (pxRxPayload);
        ucRamCounter +=
          xQueuePullAll (pxRxPayload, &ucRamBuffer[ucRamCounter]);
        vLedToggle(LED_LED2);
      }
      break;

    default:
      prvvCheckError (eStatus);
      break;
  }
  return eError;
}

// ------------------------------------------------------------------------------
// Nous sommes adressé comme esclave en lecture, nous devons envoyer des
// données
// Attention ! Fonction exécutée sous interruption
eTwiStatus
eTwiSlaveTxCB (xQueue * pxTxPayload, eTwiStatus eStatus) {
  eTwiStatus eError = TWI_SUCCESS;

  switch (eStatus) {

    case TWI_STATUS_LCALL:
      // Premier octet demandé
      // Pas de break, c'est normal !

    case TWI_STATUS_TXBUFFER_EMPTY:
      if (xQueueIsFull (pxTxPayload)) {

        eError = TWI_ERROR_BUFFER_FULL;
      }
      else {

        vQueuePush (pxTxPayload, ucRamBuffer[ucRamCounter++]);
        vLedToggle(LED_LED3);
      }
      break;

    default:
      prvvCheckError (eStatus);
      break;
  }
  return eError;
}

/* ========================================================================== */
