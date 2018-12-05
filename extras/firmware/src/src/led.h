/**
 * @file led.h
 * @brief Interface Homme-Machine Projet Toueris 2 - Partie leds
 */
#ifndef _HMI4DINBOX_LED_H_
#define _HMI4DINBOX_LED_H_

#include <avrio/defs.h>

/* internal public functions ================================================ */
void vHmiLedInit (void);
void vHmiLedToggleAll (uint8_t value);
void vHmiLedSet(uint8_t led);
void vHmiLedClear(uint8_t led);
void vHmiLedToggle(uint8_t led);
/* ========================================================================== */
#endif  /* _HMI4DINBOX_LED_H_ not defined */
