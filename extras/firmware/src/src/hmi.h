/**
 * @file hmi.h
 * @brief Interface Homme-Machine Projet Toueris 2
 * @author Copyright © 2018 epsilonRT. All rights reserved.
 */
#ifndef _HMI4DINBOX_H_
#define _HMI4DINBOX_H_

#include <avrio/defs.h>
#include <avrio/eeprom.h>
//#include <avrio/mutex.h>
//#include "version-git.h"

/* structures =============================================================== */
typedef struct xHmiConfig {

  uint8_t ucBacklight;
} xHmiConfig;

typedef struct xHmiConfigEE {

  xHmiConfig xConfig;
  uint8_t ucCrc;
} xHmiConfigEE;

/* extern variables ========================================================= */
//extern uint8_t ucFlag;
//extern xButMask xButtons;
//extern xMutex xHmiMutex;

/* internal public functions ================================================ */
void vHmiInit(void);
void vHmiLoop(void);

/* ========================================================================== */
#endif  /* _HMI4DINBOX_H_ not defined */
