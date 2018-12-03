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
#ifndef _AVRIO_BOARD_BUTTON_H_
#  define _AVRIO_BOARD_BUTTON_H_
/* ========================================================================== */

/* BUTTON==================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>
#  include <avr/interrupt.h>

/* constants ================================================================ */
#  define BUTTON_QUANTITY  5

// PB4-PB7
#  define BUTTON_BUTTON1 _BV(4)
#  define BUTTON_BUTTON3 _BV(6)
#  define BUTTON_BUTTON4 _BV(7)
#  define BUTTON_BUTTON5 _BV(5)
#  define BUTTON_PORTB   (BUTTON_BUTTON1|BUTTON_BUTTON3|BUTTON_BUTTON4|BUTTON_BUTTON5)

// PD6
#  define BUTTON_BUTTON2 _BV(3)
#  define BUTTON_PORTD_MASK(b) ((b)<<3)
#  define BUTTON_PORTD_VALUE(b) ((b)>>3)
#  define BUTTON_PORTD   (BUTTON_PORTD_MASK(BUTTON_BUTTON2))

#  define BUTTON_ALL_BUTTONS (BUTTON_PORTB|BUTTON_BUTTON2)
#  define BUTTON_NO_BUTTON (0)

// PCINT
#  define BUTTONB_vect PCINT0_vect
#  define BUTTONB_PCIE PCIE0
#  define BUTTONB_PCMSK  PCMSK0
#  define BUTTON_BUTTON1_PCINT  PCINT4
#  define BUTTON_BUTTON2_PCINT  
#  define BUTTON_BUTTON3_PCINT  PCINT6
#  define BUTTON_BUTTON4_PCINT  PCINT7
#  define BUTTON_BUTTON5_PCINT  PCINT5

#  if !defined(PCICR) && defined(GIMSK)
#   define PCICR GIMSK
#  endif
/* types ==================================================================== */
typedef uint8_t xButMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vButHardwareInit (void) {

  PORTB |= BUTTON_PORTB;
  DDRB &= ~BUTTON_PORTB;
  PORTD |= BUTTON_PORTD;
  DDRD &= ~BUTTON_PORTD;
}

// ------------------------------------------------------------------------------
static inline xButMask
xButHardwareGet (xButMask xMask) {

  return (BUTTON_PORTD_VALUE((PIND ^ BUTTON_PORTD) & BUTTON_PORTD) | ((PINB ^ BUTTON_PORTB) & BUTTON_PORTB)) & xMask;
}

/* public variables ========================================================= */
#  if defined(BUTTON_MASK_ARRAY_ENABLE)
#    define DECLARE_BUTTON_MASK_ARRAY  \
  const xButMask \
    xButMaskArray [BUTTON_QUANTITY] = { \
      BUTTON_BUTTON1,BUTTON_BUTTON2,BUTTON_BUTTON3,BUTTON_BUTTON4,BUTTON_BUTTON5 \
    }
#  else
#    define DECLARE_BUTTON_MASK_ARRAY
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BUTTON_H_ */
