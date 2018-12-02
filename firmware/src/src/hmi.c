/**
 * @file thmi.c
 * @brief Interface Homme-Machine Projet Toueris 2
 */
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avrio/twi.h>
#include <avrio/delay.h>
#include "hmi.h"
#include "led.h"
#include "backlight.h"
#include "hirq.h"
#include "button.h"
#include <Hmi4DinBox.h>
#include "config.h" // toujours en dernier

/* constants ================================================================ */

/* public variables ========================================================= */
xHmiConfig xConfig;
xHmiConfigEE xConfigEE EEMEM;

/* private variables ======================================================== */
static uint8_t ucI2cBuffer[2];
static volatile uint8_t ucBufferIdx; // Index sur buffer I2c
static uint8_t ucPreviousBacklight;

/* private functions ======================================================== */
void vAssert (bool bTest);
void vWaitUntilI2cBusy (void);

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vHmiInit (void) {
  xLedMask led;
  MCUSR = 0;  // Permet de dévalider le watch-dog en cas de reset watch-dog

  wdt_disable();  // Evite un déclenchement watch-dog pendant Init

  vLedInit();
  vHmiHirqInit();
  if (MCUSR & _BV (WDRF)) {

    // Reset déclenché par Watchdog
    cli();
    vHmiHirqClear();
    led = LED_RED;
  }
  else {

    led = LED_GREEN1;
  }
  vLedSet (led);

  if (iEepromLoadBlock (&xConfig, &xConfigEE, sizeof (xHmiConfig)) < 0) {

    // Si le contenu de xConfigEE est corrompu, réinitialiser avec les
    // valeurs par défaut.
    xConfig.ucBacklight = CFG_LCD_BACKLIGHT;
    vEepromSaveBlock (&xConfig, &xConfigEE, sizeof (xHmiConfig));
  }

  vHmiButtonInit();
  vHmiBacklightInit();
  vHmiBacklightSet (xConfig.ucBacklight);
  ucPreviousBacklight = xConfig.ucBacklight;

  vTwiInit ();
  vTwiSetDeviceAddress (HMI4DINBOX_SLAVE_ADDR << 1);

  vHmiHirqClear();
  MCUSR |= _BV (WDRF); // Clear du bit Watchdog Reset Flag

  vLedClear (led);
  // Signale fin init en faisant clignoter 3 fois, la led rouge si déclenchement
  // watchdog, sinon la led verte
  for (uint8_t j = 0; j < (3 * 2); j++) {

    vLedToggle (led);
    delay_ms (50);
  }
  wdt_enable (CFG_IHM_WDT_TIMEOUT); // Sécurité en cas de blocage appli.
  sei(); // Valide les interruptions.
}

// -----------------------------------------------------------------------------
void
vHmiLoop (void) {

  vHmiButtonTask();
  if (ucPreviousBacklight != xConfig.ucBacklight) {

    vHmiBacklightSet (xConfig.ucBacklight);
    ucPreviousBacklight = xConfig.ucBacklight;
    vEepromSaveBlock (&xConfig, &xConfigEE, sizeof (xHmiConfig));
  }
  wdt_reset();
}

/*------------------------------------------------------------------------------
 * Gestionnaire de réception I2C
 * Nous sommes adressé comme esclave en écriture, nous avons reçu une trame
 * qui se trouve dans la pile pxRxPayload.
 * Attention ! Fonction exécutée sous interruption, il faut être bref
 */
eTwiStatus
eTwiSlaveRxCB (xQueue * pxRxPayload, eTwiStatus eStatus) {
  eTwiStatus eError = TWI_SUCCESS;

  switch (eStatus) {

    case TWI_STATUS_GCALL:
      // Ecriture en appel général non implémenté !
      eError = TWI_ERROR_ADDR_NACK;
      break;

    case TWI_STATUS_LCALL: {
      uint8_t value;

      // Adressage avec notre adresse esclave
      if (xQueueIsEmpty (pxRxPayload)) {

        return eError;
      }

      /*
       * Le premier octet reçu est l'adresse du registre auquel le
       * maître souhaite accéder. Si la trame en écriture à une longeur de
       * 1 octet, seul l'adresse du registre est modifiée. Ce sera
       * généralement le cas, lors d'un accès "write word address; READ data"
       * permettant au maître de spécifier l'adresse du registre qu'il
       * souhaite lire avant de les lire.
       */
      value = ucQueuePull (pxRxPayload);
      if ( (value == LED_REG) || (value == BACKLIGHT_REG)) {

        ucBufferIdx = value;
        while (! xQueueIsEmpty (pxRxPayload)) {

          value  = ucQueuePull (pxRxPayload);
          switch (ucBufferIdx) {

            case LED_REG:
              value &= (_BV (LED1) | _BV (LED2) | _BV (LED3) | _BV (LED4) | _BV (LED5));
              if (value != ucI2cBuffer[ucBufferIdx]) {

                vHmiLedToggleAll (value ^ ucI2cBuffer[ucBufferIdx]);
                ucI2cBuffer[ucBufferIdx] = value;
              }
              break;

            case BACKLIGHT_REG:
              if (value != ucI2cBuffer[ucBufferIdx]) {
                ucI2cBuffer[ucBufferIdx] = value;
                xConfig.ucBacklight = value;
              }
              break;

            default:
              break;
          }
        }
        return eError;
      }
      else {

        eError = TWI_ERROR_DATA_NACK;
      }
    }
    // Pas de break, normal !

    default:
      vQueueFlush (pxRxPayload);
      break;
  }
  return eError;
}

/*------------------------------------------------------------------------------
 * Gestionnaire d'émission I2C
 * Nous sommes adressé comme esclave en lecture, nous devons envoyer des
 * données qui devront être mis dans la pile pxTxPayload.
 * Attention ! Fonction exécutée sous interruption, il faut être bref
 */
eTwiStatus
eTwiSlaveTxCB (xQueue * pxTxPayload, eTwiStatus eStatus) {

  switch (eStatus) {

    case TWI_STATUS_LCALL:
      // Adressage avec notre adresse esclave
      // Premier octet demandé

      // Pas de break, c'est normal !

    case TWI_STATUS_TXBUFFER_EMPTY:
      // Le buffer de transmission est vide, il faut ajouter des octets

      switch (ucBufferIdx) {

        case LED_REG:
        case BACKLIGHT_REG:
          vQueuePush (pxTxPayload, ucI2cBuffer[ucBufferIdx]);
          break;

        case KEY_REG:
          if (bHmiButtonAvailable()) {

            xHmiButtonRead (pxTxPayload);
          }
          else {

            vQueuePush (pxTxPayload, 0);
          }
          break;

        default:
          return TWI_ERROR_DATA_NACK;
      }
      break;

    default:
      break;
  }
  return TWI_SUCCESS;
}

// -----------------------------------------------------------------------------
// Vérifies l'absence d'erreur, si ce n'est pas le cas, bloque l'exécution
// en faisant flasher la led rouge en mode DEBUG ou allume la led rouge en mode
// RELEASE.
void
vAssert (bool bTest) {

#ifdef DEBUG
  if (!bTest) {

    for (;;) {

      vLedSet (LED_RED);
      delay_ms (50);
      vLedClear (LED_RED);
      delay_ms (150);
    }
  }
#else
  if (!bTest) {
    vLedSet (LED_RED);
  }
#endif
}

// -----------------------------------------------------------------------------
void
vWaitUntilI2cBusy (void) {

  do {

    delay_ms (50);
  }
  while (bTwiIsBusy());
}

/* ========================================================================== */
