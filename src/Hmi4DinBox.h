/*
   Copyright (c) 2018-2023, Pascal JEAN aka epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
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
#include <WireLcd.h>

class Hmi4DinBox : public  WireSlave {
  public:
    Hmi4DinBox (int hirqPin = -1, byte slaveAddress = HMI4DINBOX_SLAVE_ADDR);
    bool begin (int lcdContrast = 25, bool lcdBoost = false); // contrast value range is 0-63, try 25@5V or 50@3.3V as a starting value
    WireLeds led;
    WireKeyboard keyb;
    WireBackLight backlight;
    WireLcd lcd;
};

#endif /* __cplusplus defined */

/* ========================================================================== */
#endif /* __HMI4DINBOX_PUBLIC_H__ */
