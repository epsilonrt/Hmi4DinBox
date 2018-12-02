/**
 * @file button.h
 * @brief Interface Homme-Machine Projet Toueris 2 - Partie bouton poussoir
 */
#ifndef _TOUERIS2_BUTTON_H_
#define _TOUERIS2_BUTTON_H_

#include <avrio/defs.h>
#include <avrio/button.h>
#include <avrio/queue.h>

/* internal public functions ================================================ */
void vHmiButtonInit(void);
void vHmiButtonTask (void);
bool bHmiButtonAvailable (void);
size_t xHmiButtonRead (struct xQueue *pxDstQueue);

/* ========================================================================== */
#endif  /* _TOUERIS2_BUTTON_H_ not defined */
