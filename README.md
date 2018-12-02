# Hmi4DinBox  
_Human-Machine Interface for Din Box_

<a href="https://www.oshwa.org/">
  <img src="https://raw.githubusercontent.com/epsilonrt/Hmi4DinBox/master/images/Open-source-hardware-logo.png" alt="Open-source-hardware-logo.png" align="right" valign="top">
</a>

## Features

![Hmi4DinBox in his box.](https://github.com/epsilonrt/Hmi4DinBox/raw/master/images/hmi4dinbox.png)

Hmi4DinBox is a human-machine interface designed to be implemented on the front 
of a DIN rail enclosure.

Its features are as follows :   
* 5 leds,  
* a navigation button with (4 directions + 1 push button),  
* an 2x16 LCD display with a backlight (based on ST7032),  
* driven by a simple I2C bus and a binary signal `HIRQ` which indicates that one or 
more actions have been performed on the navigation button (key presses and 
releases in the 5 directions stored in a buffer),  
* powered by 5V or 3.3V allowing it to be driven by an Arduino board or Pi board.

Published in open-source hardware, it comes with its 
[electronic schema](https://github.com/epsilonrt/Hmi4DinBox/raw/master/hmi4dinbox-sch.pdf), 
[layout](https://github.com/epsilonrt/Hmi4DinBox/raw/master/hmi4dinbox-layout.pdf) 
and [manufacturing files](https://github.com/epsilonrt/Hmi4DinBox/tree/master/gerber) and a control library.

[Hmi4DinBox Animation](https://raw.githubusercontent.com/epsilonrt/Hmi4DinBox/master/images/hmi4dinbox.webm)

## How to connect your HMI to your board ?

**TODO**

## How to install the library ?

This library allows the client-side control of the Hmi4DinBox connected by an I2C bus. 
It is intended to be used in an Arduino environment (also compatible with the 
[PiDuino library](https://github.com/epsilonrt/piduino) on Pi boards such as Raspberry Pi or NanoPi).

Associated with a library for the management of the LCD like [LCD_ST7032](https://github.com/epsilonrt/LCD_ST7032), 
one has a complete toolbox to control the Hmi4DinBox.

## How to use the HMI in your program ?

At the beginning of the file 
[Hmi4DinBox.h](https://github.com/epsilonrt/Hmi4DinBox/blob/master/library/src/Hmi4DinBox.h), 
we see the definition of constants describing the hardware solution:

    #define LED1 0
    #define LED2 1
    #define LED3 2
    #define LED4 3
    #define LED5 4

We have 5 leds, the numbering starts at 0.

    #define LED_RED LED1
    #define LED_GREEN1 LED2
    #define LED_GREEN2 LED3
    #define LED_YELLOW1 LED4
    #define LED_YELLOW2 LED5

We define more explicit constants corresponding to the colors of the leds: the 
led N° 1 is red, the N° 2 and 3 green ...

Then we find the constants for the keyboard function:

    #define KUP 1
    #define KDOWN 2
    #define KLEFT 3
    #define KRIGHT 4
    #define KCENTER 5

We have 5 keys corresponding to the 4 directions of the navigation button and 
the center button.

To use this HMI in a program, you must declare a global variable of type Hmi4DinBox:

    Hmi4DinBox hmi;

If one uses the navigation pad, it is necessary during this declaration to 
specify the number of pin Arduino used by the HMI to indicate that keys are in 
the internal buffer (it will thus be necessary to read them ...).

    const int hirqPin = 2;
    Hmi4DinBox hmi (hirqPin);

Then we can access:
* to the led function thanks to `hmi.led`,
* to the keyboard function thanks to `hmi.keyb`,
* the backlight function with `hmi.backlight`.

### Using leds

As can be seen in the sketch [LedDemo.ino](https://github.com/epsilonrt/Hmi4DinBox/blob/master/library/examples/Hmi4DinBox/LedDemo/LedDemo.ino):

    hmi.led.set (LED1);

turn on the LED1,

    hmi.led.clear (LED1);

turn off the LED1,

    hmi.led.toggle (LED1);

toggles the state of LED1,

    hmi.led.get (LED1);

read the state of the LED1,

The `hmi.led.writeAll ()` function allows you to modify all leds at the same time. By default, a call to this function without parameters turns on all leds.
If we pass him a parameter, that corresponds to the state of the leds. Bit 0 of this parameter is used to control LED1, bit 1, LED2 ...

### Using the keyboard

As can be seen in the sketch [KeyboardDemo.ino](https://github.com/epsilonrt/Hmi4DinBox/blob/master/library/examples/Hmi4DinBox/KeyboardDemo/KeyboardDemo.ino):

    hmi.keyb.available ()

to test if keys are available for reading,

    key = hmi.keyb.key ();

allows you to read the next key,

    hmi.keyb.released ()

lets you know if this key has been released and,

    hmi.keyb.pressed ()

if she was pressed.

### Using the backlight

As can be seen in the sketch [BacklightDemo.ino](https://github.com/epsilonrt/Hmi4DinBox/blob/master/library/examples/Hmi4DinBox/BacklightDemo/BacklightDemo.ino):

    hmi.backlight.write (bl);

allows to modify the value of the backlight (between 0 and 255), as this value is stored in EEPROM by the HMI, it is possible to read the current value thanks to:

    bl = hmi.backlight.read ();

------
