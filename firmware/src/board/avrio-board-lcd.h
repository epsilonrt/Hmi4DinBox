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
#ifndef _AVRIO_BOARD_LCD_ST7032_H_
#define _AVRIO_BOARD_LCD_ST7032_H_

#include <avrio/lcd/lcd_defs.h>
#include <avr/io.h>

/* Interface matérielle ===================================================== */
/* Choix de l'interface reliant l'afficheur au microcontrôleur LCD_IO :
 * Valeurs possibles:
 * LCD_IO_PIO_4       -> Interface sur port IO avec bus afficheur 4 bits
 * LCD_IO_PIO_8       -> Interface sur port IO avec bus afficheur 8 bits
 * LCD_IO_TWI         -> Interface I²C native (Le contrôleur intégré est I²C)
 * LCD_TWI_PCF8574 -> Afficheur en 4 bits relié à un PCF8574 (I²C)
 */
#define LCD_IO LCD_IO_TWI

/* Valide la compilation et l'utilisation de la lecture dans l'afficheur (BF) */
//#define LCD_IO_READ_ENABLE

/* Valide la prise en charge du rétro-éclairage (avrio-board-lcdbacklight.h) */
#define LCD_CTRL_BACKLIGHT_ENABLE

/* Broche de RESET (effectué à l'init si LCD_IO_RST_BIT défini */
#define LCD_IO_RST_BIT    2
/* Port utilisé par la broche de RESET */
#define LCD_IO_RST_DDR    DDRD
#define LCD_IO_RST_PORT   PORTD
/* Niveau d'activation du RESET
 * Actif à l'état haut = 1
 * Actif à l'état bas  = 0
 */
#define LCD_IO_RST_LEVEL  0
/* Durée d'impulsion de RESET en ms */
#define LCD_IO_RST_PW     10
/* Temps d'attente après de RESET */
#define LCD_IO_RST_DELAY  100

/* Interface LCD_IO_PIO ============ */

/* Valide l'inversion du bus de données pour faciliter le routage du PCB :
 * DB7->DB0...DB0->DB7 en 8 bits
 * DB7->DB4...DB4->DB7 en 4 bits
 */
//#define LCD_PIO_DATA_REVERSE

/* Largeur de l'impulsion de validation sur E en µs */
#define LCD_PIO_E_PW       1

/* - Numéro des bits de port utilisés en PIO - */
#define LCD_PIO_RS_BIT   6
#define LCD_PIO_E_BIT    7
//#define LCD_PIO_RW_BIT   6
//#define LCD_PIO_RD_BIT   7

/* - Ports utilisés en PIO - */
#define LCD_PIO_DATA_PORT  PORTA
#define LCD_PIO_DATA_DDR   DDRA

#define LCD_PIO_RS_PORT    PORTC
#define LCD_PIO_RS_DDR     DDRC

#define LCD_PIO_E_PORT     PORTC
#define LCD_PIO_E_DDR      DDRC

//#define LCD_PIO_RW_PORT    PORTD
//#define LCD_PIO_RW_DDR     DDRD

//#define LCD_PIO_RD_PORT    PORTD
//#define LCD_PIO_RD_DDR     DDRD

/* -- Interface LCD_IO_PIO_4 ======= */
/* - Numéro du fil du port relié à DB4 en PIO4 - */
#define LCD_PIO_DATA_DB4 4

/* Interface LCD_IO_TWI ============ */
/* Vitesse du bus I2c en KHz, si définie, le module LCD initialise le module
 * TWI maître avec cette vitesse, sinon, il est du ressort de l'utilisateur
 * d'initialiser le module TWI maître AVANT d'initialiser le module LCD.
 */
#define LCD_TWI_MASTER_INIT 400

/* Adresse I2c du circuit contrôlant l'afficheur */
#define LCD_TWI_DEVICE_ADDR (ST7032_BASE)

/* -- Interface LCD_TWI_PCF8574 = */
/* Valide l'inversion du bus de données pour faciliter le routage du PCB :
 * DB7->DB0...DB0->DB7 en 8 bits
 * DB7->DB4...DB4->DB7 en 4 bits
 */
//#define LCD_PCF8574_DATA_REVERSE

/* - Numéro des bits de port utilisés avec PCF8574 - */
#define LCD_PCF8574_RS_BIT   6
#define LCD_PCF8574_E_BIT    7
#define LCD_PCF8574_DATA_DB4 4
//#define LCD_PCF8574_RW_BIT   6

/* Contrôleur Afficheur ===================================================== */
/* Choix du contrôleur LCD_CTRL:
 * Valeurs possibles:
 * - LCD_CTRL_HD44780
 * - LCD_CTRL_PCF2119
 * - LCD_CTRL_ST7032
 * - LCD_CTRL_THMI
 */
#define LCD_CTRL LCD_CTRL_ST7032

/* Validation de la prise en charge de l'affichage d'icônes */
#define LCD_CTRL_ICON_ENABLE

/* Valide la prise en charge du contraste */
//#define LCD_CTRL_CONTRAST_ENABLE

/* Contrôleur HD44780 =============== */
/* Configuration Function Set: HD44780_FSET
 * + HD44780_DL -> DL=0 si interface 4 bits / DL=1 si interface 8 bits
 * + HD44780_N  -> N=0 1 ligne              / N=1 2 lignes
 * + HD44780_F  -> F=0 Caractère 5x8        / F=1 Caractère 5x10 (uniquement en N=0)
 */
#define LCD_CTRL_HD44780_INIT_FSET (HD44780_FSET + HD44780_N)

/* Configuration Display On/Off Control: HD44780_DCTL
 * + HD44780_D -> D=1 Afficheur allumé   / D=0 éteint
 * + HD44780_C -> C=1 Curseur affiché    / C=0 pas de curseur
 * + HD44780_B -> B=1 Curseur clignotant / B=0 Curseur _
 */
#define LCD_CTRL_HD44780_INIT_DCTL (HD44780_DCTL + HD44780_D)

/* Configuration Entry Mode Set: HD44780_ENTM
 * + HD44780_ID -> I/D=0 curseur vers la gauche / I/D=1 vers la droite
 * + HD44780_S  -> S  =0 Le curseur se décale   / S  =1 c'est tout l'afficheur
 */
#define LCD_CTRL_HD44780_INIT_ENTM (HD44780_ENTM + HD44780_ID)

/* Contrôleur PCF2119 =============== */
/* Table de caractères de l'afficheur
 * - A: ASCII
 * - D: ASCII tout majuscule
 * - F,R,S: non ASCII, table partielle de 0x20 à 0x7A décalée de 0x80
 */
#define LCD_CTRL_PCF2119_CHAR_SET 'R'

/* Valeur minimale dans le registre de réglage de la tension VA qui permet
 * de régler le contraste à l'aide du générateur de tension interne
 * 0 dévalide le générateur interne
 */
#define LCD_CTRL_PCF2119_VA_MIN 23

/* Configuration Function Set: PCF2119_FSET
 * + PCF2119_DL -> DL=0 si interface 4 bits / DL=1 si interface 8 bits (I²C)
 * + PCF2119_M  -> M=0 1 ligne              / M=1 2 lignes
 * + PCF2119_SL -> Mode Single Line Low current actif si SL=1 (économe en énergie)
 */
#define LCD_CTRL_PCF2119_INIT_FSET (PCF2119_FSET + PCF2119_DL + PCF2119_M)

/* Configuration Display On/Off Control: PCF2119_DCTL
 * + PCF2119_D -> D=1 Afficheur allumé   / D=0 éteint
 * + PCF2119_C -> C=1 Curseur affiché    / C=0 pas de curseur
 * + PCF2119_B -> B=1 Curseur clignotant / B=0 Curseur _
 */
#define LCD_CTRL_PCF2119_INIT_DCTL (PCF2119_DCTL + PCF2119_D)

/* Configuration Entry Mode Set: PCF2119_ENTM
 * + PCF2119_ID -> I/D=0 curseur vers la gauche / I/D=1 vers la droite
 * + PCF2119_S  -> S  =0 Le curseur se décale   / S  =1 c'est tout l'afficheur
 */
#define LCD_CTRL_PCF2119_INIT_ENTM (PCF2119_ENTM + PCF2119_ID)

/* Contrôleur ST7032 =============== */

/* Valeur de la tension d'alimentation de l'afficheur en millivolts
 * Permet de régler la génération interne de la tension LCD (V0)
 * 0 dévalide la génération interne de la tension LCD
 */
#define LCD_CTRL_ST7032_VDD 5000

/* Configuration Function Set: ST7032_FSET
 * + ST7032_DL -> DL=0 si interface 4 bits / DL=1 si interface 8 bits (I²C)
 * + ST7032_N  -> N=0 1 ligne              / N=1 2 lignes
 * + ST7032_DH -> Mode Fonte Double (5x16) actif si DH=1
 */
#define LCD_CTRL_ST7032_INIT_FSET (ST7032_FSET + ST7032_DL + ST7032_N)

/* Configuration Display On/Off Control: ST7032_DCTL
 * + ST7032_D -> D=1 Afficheur allumé   / D=0 éteint
 * + ST7032_C -> C=1 Curseur affiché    / C=0 pas de curseur
 * + ST7032_B -> B=1 Curseur clignotant / B=0 Curseur _
 */
#define LCD_CTRL_ST7032_INIT_DCTL (ST7032_DCTL + ST7032_D)

/* Configuration Entry Mode Set: ST7032_ENTM
 * + ST7032_ID -> I/D=0 curseur vers la gauche / I/D=1 vers la droite
 * + ST7032_S  -> S  =0 Le curseur se décale   / S  =1 c'est tout l'afficheur
 */
#define LCD_CTRL_ST7032_INIT_ENTM (ST7032_ENTM + ST7032_ID)

/* Contrôleur THMI =============== */
/* Configuration Function Set: THMI_FSET
 * + THMI_DL -> DL=0 si interface 4 bits / DL=1 si interface 8 bits
 * + THMI_N  -> N=0 1 ligne              / N=1 2 lignes
 * + THMI_F  -> F=0 Caractère 5x8        / F=1 Caractère 5x10 (uniquement en N=0)
 */
#define LCD_CTRL_THMI_INIT_FSET (THMI_FSET + THMI_N)

/* Configuration Entry Mode Set: THMI_ENTM
 * + THMI_ID -> I/D=0 curseur vers la gauche / I/D=1 vers la droite
 * + THMI_S  -> S  =0 Le curseur se décale   / S  =1 c'est tout l'afficheur
 */
#define LCD_CTRL_THMI_INIT_ENTM (THMI_ENTM + THMI_ID)

/* Configuration Display On/Off Control: THMI_DCTL
 * + THMI_D -> D=1 Afficheur allumé   / D=0 éteint
 * + THMI_C -> C=1 Curseur affiché    / C=0 pas de curseur
 * + THMI_B -> B=1 Curseur clignotant / B=0 Curseur _
 */
#define LCD_CTRL_THMI_INIT_DCTL (THMI_DCTL + THMI_D)


/* Afficheur ================================================================ */
/* Nombre de lignes */
#define LCD_HEIGHT 2
/* Nombre de caractères par ligne */
#define LCD_WIDTH  16
/* Largeur d'un caractère en pixels */
#define LCD_PATTERN_WIDTH 5
/* Hauteur d'un caractère en pixels */
#define LCD_PATTERN_HEIGHT 8

/* Contraste par défaut */
#define LCD_DEFAULT_CONTRAST 16

/* Valide la compilation des fonctions de bargraph */
//#define LCD_BARGRAPH_ENABLE

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_LCD_ST7032_H_ not defined */
