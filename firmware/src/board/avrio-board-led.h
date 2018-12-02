/*
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#ifndef _AVRIO_BOARD_LED_H_
#  define _AVRIO_BOARD_LED_H_
/* ========================================================================== */

/* LED ====================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define LED_QUANTITY  5
#  define LED_LED1 _BV(3)
#  define LED_LED2 _BV(7)
#  define LED_LED3 _BV(5)
#  define LED_LED4 _BV(4)
#  define LED_LED5 _BV(0)

#  define LED_PORTB     (LED_LED5)
#  define LED_PORTD     (LED_LED1 | LED_LED2 | LED_LED3 | LED_LED4)
#  define LED_ALL_LEDS  (LED_PORTD | LED_PORTB)
#  define LED_NO_LED (0)

/* types ==================================================================== */
typedef uint8_t xLedMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vLedClear (xLedMask xMask) {

  PORTB &= ~(xMask & LED_PORTB);
  PORTD &= ~(xMask & LED_PORTD);
}

// ------------------------------------------------------------------------------
static inline void
vLedInit (void) {

  DDRB |= LED_PORTB;
  DDRD |= LED_PORTD;
  vLedClear (LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedSet (xLedMask xMask) {

  PORTB |= (xMask & LED_PORTB);
  PORTD |= (xMask & LED_PORTD);
}

// ------------------------------------------------------------------------------
static inline void
vLedToggle (xLedMask xMask) {

  PORTB ^= (xMask & LED_PORTB);
  PORTD ^= (xMask & LED_PORTD);
}

/* public variables ========================================================= */
#  if defined(LED_MASK_ARRAY_ENABLE)
#    define DECLARE_LED_MASK_ARRAY  \
  const xLedMask \
  xLedMaskArray [LED_QUANTITY] = { \
                                   LED_LED1, LED_LED2, LED_LED3, LED_LED4, LED_LED5  \
                                 }
#  else
#    define DECLARE_LED_MASK_ARRAY
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LED_H_ */
