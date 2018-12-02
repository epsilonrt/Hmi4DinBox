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
#ifndef _AVRIO_BOARD_LCDBACKLIGHT_H_
#  define _AVRIO_BOARD_LCDBACKLIGHT_H_
/* ========================================================================== */

/* LCD_BACKLIGHT ============================================================ */
#  include "backlight.h"

/* inline public functions ================================================== */
/*
 * Initialise la ressource matérielle (timer PWM de préférence) utilisée par
 * le module lcd (rétroéclairage).
 * A modifier en fonction du timer et de la sortie utilisée.
 */
INLINE void
vLcdDriverBacklightInit (void) {

  vHmiBacklightInit();
}

/*
 * Modifie le niveau de rétroéclairage du LCD
 * @param ucValue niveau entre 0 et 63
 */
INLINE void
vLcdDriverBacklightSet (uint8_t ucValue) {

  vHmiBacklightSet (ucValue << 2); // x4
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LCDBACKLIGHT_H_ */
