/**
 * @file config.h 
 * @brief Configuration de ihm
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id: config.h 4 2013-01-28 09:43:38Z pjean $
 * Revision History ---
 *    20121212 - Initial version by epsilonRT
 */
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* constants ================================================================ */
#define CFG_LCD_BACKLIGHT 8
/*
 * Timeout Watch-Dog avant RESET
 *
 * Tableau des valeurs possibles
 * -------------------------------
 * Constante  WDP  Cycles  Timeout
 * ================================
 * WDTO_15MS  0    2K      16ms
 * WDTO_30MS  1    4K      32ms
 * WDTO_60MS  2    8K      64ms
 * WDTO_120MS 3    16K     0.125 s
 * WDTO_250MS 4    32K     0.25 s
 * WDTO_500MS 5    64K     0.5 s
 * WDTO_1S    6    128K    1.0 s
 * WDTO_2S    7    256K    2.0 s
 * WDTO_4S    8    512K    4.0 s
 * WDTO_8S    9     1024K  8.0 s
 * ================================
 */
#define CFG_IHM_WDT_TIMEOUT WDTO_500MS

/*
 * Taille du buffer FIFO des boutons
 */
#define CFG_BUT_FIFO_SIZE 32

/* ========================================================================== */
#endif  /* _CONFIG_H_ not defined */
