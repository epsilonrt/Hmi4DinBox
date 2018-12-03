/* Copyright © 2018 Pascal JEAN, All rights reserved.
 * This file is part of the Hmi4DinBox Library.
 *
 * The Hmi4DinBox Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * The Hmi4DinBox Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the Hmi4DinBox Library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "Hmi4DinBox.h"

// -----------------------------------------------------------------------------
//
//                            Hmi4DinBox class
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
Hmi4DinBox::Hmi4DinBox (int hirqPin, byte slaveAddress) :
  WireSlave (slaveAddress),
  led (this, LED_REG, LED_NB),
  keyb (this, KEY_REG, hirqPin),
  backlight (this, BACKLIGHT_REG) {
}

// -----------------------------------------------------------------------------
bool Hmi4DinBox::begin() {
  
  return this->led.begin() && this->keyb.begin() && this->backlight.begin();
}

/* ========================================================================== */
