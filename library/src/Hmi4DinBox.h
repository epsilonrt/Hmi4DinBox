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
#ifndef __HMI4DINBOX_PUBLIC_H__
#define __HMI4DINBOX_PUBLIC_H__

#define LED1  0
#define LED2  1
#define LED3  2
#define LED4  3
#define LED5  4

#define LED_RED     LED1
#define LED_GREEN1  LED2
#define LED_GREEN2  LED3
#define LED_YELLOW1 LED4
#define LED_YELLOW2 LED5

#define KUP       1
#define KDOWN     2
#define KLEFT     3
#define KRIGHT    4
#define KCENTER   5

#define HMI4DINBOX_SLAVE_ADDR 0b0111111

#define LED_NB        5
#define LED_REG       0
#define BACKLIGHT_REG 1
#define KEY_REG       2
#define KEY_RELEASE   (1<<7)

#ifdef __cplusplus

#include <WireHmi.h>

class Hmi4DinBox : public  WireSlave {
  public:
    Hmi4DinBox (int hirqPin = -1, byte slaveAddress = HMI4DINBOX_SLAVE_ADDR);
    bool begin();
    WireLeds led;
    WireKeyboard keyb;
    WireBackLight backlight;
};

#endif /* __cplusplus defined */

/* ========================================================================== */
#endif /* __HMI4DINBOX_PUBLIC_H__ */
