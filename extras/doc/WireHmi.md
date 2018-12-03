# WireHmi Library

## Abstract

This library allows the client-side control of a human-machine interface (HMI) connected by an I2C bus. It is intended to be used in an Arduino environment (also compatible with the [PiDuino library](https://github.com/epsilonrt/piduino) on Pi boards such as Raspberry Pi or NanoPi).

The WireHmi library is designed in a modular way to create objects (C ++ classes) corresponding to any hardware solution.

The available modules are:

* `WireSlave` which corresponds to the modeling of any I2C slave. All HMI classes created by the user will be derived from this class.  
* `WireRegister` which corresponds to the modeling of a register that controls a functionality in a slave (module). All feature modules (`WireLeds`, `WireKeyboard` ...) supplied by the WireHmi library and all those that will be created by the user will be derived from this class.  
* `WireLeds` that can manage up to 8 LEDs (ON / OFF control).  
* `WireKeyboard` which allows to manage a set of push buttons (up to 127) with possibility to manage an interruption signal.  
* `WireBackLight` that manages the backlight intensity of an LCD (or any element that can vary between 0 and 255).

Associated with a library for the management of the LCD like [LCD_ST7032](https://github.com/epsilonrt/LCD_ST7032), one has a complete toolbox to create any HMI.

## Example of using an HMI (for the **Toueris** project)

The human-machine interface of the MODBUS slave modules of the Toueris project has:

* 5 leds
* a navigation pad with (4 directions + 1 push button) and
* an LCD display with a backlight (based on ST7032)

At the beginning of the file [Hmi4DinBox.h](https://github.com/epsilonrt/WireHmi/blob/master/src/Hmi4DinBox.h), we see the definition of constants describing the hardware solution:

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

We define more explicit constants corresponding to the colors of the leds: the led N° 1 is red, the N° 2 and 3 green ...

Then we find the constants for the keyboard function:

    #define KUP 1
    #define KDOWN 2
    #define KLEFT 3
    #define KRIGHT 4
    #define KCENTER 5

We have 5 keys corresponding to the 4 directions of the navigation button and the center button.

To use this HMI in a program, you must declare a global variable of type Toueris2Hmi:

    Toueris2Hmi hmi;

If one uses the navigation pad, it is necessary during this declaration to specify the number of pin Arduino used by the HMI to indicate that keys are in the internal buffer (it will thus be necessary to read them ...).

    const int hirqPin = 2;
    Toueris2Hmi hmi (hirqPin);

Then we can access:
* to the led function thanks to `hmi.led`,
* to the keyboard function thanks to `hmi.keyb`,
* the backlight function with `hmi.backlight`.

### Using leds

As can be seen in the sketch [LedDemo.ino](https://github.com/epsilonrt/WireHmi/blob/master/examples/Toueris2Hmi/LedDemo/LedDemo.ino):

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

As can be seen in the sketch [KeyboardDemo.ino](https://github.com/epsilonrt/WireHmi/blob/master/examples/Toueris2Hmi/KeyboardDemo/KeyboardDemo.ino):

    hmi.keyb.available ()

to test if keys are available for reading,

    key = hmi.keyb.key ();

allows you to read the next key,

    hmi.keyb.released ()

lets you know if this key has been released and,

    hmi.keyb.pressed ()

if she was pressed.

### Using the backlight

As can be seen in the sketch [BacklightDemo.ino](https://github.com/epsilonrt/WireHmi/blob/master/examples/Toueris2Hmi/BacklightDemo/BacklightDemo.ino):

    hmi.backlight.write (bl);

allows to modify the value of the backlight (between 0 and 255), as this value is stored in EEPROM by the HMI, it is possible to read the current value thanks to:

bl = hmi.backlight.read ();

## HMI creation example (for **Toueris** project)

This part is about developers who want to create a new interface. We take the case of the HMI Toueris.

At the beginning of the file [Hmi4DinBox.h](https://github.com/epsilonrt/WireHmi/blob/master/src/Hmi4DinBox.h), we see:

    #define HMI4DINBOX_SLAVE_ADDR 0b0111111

which corresponds to the slave address of this HMI. This address is used by default by the constructor of the class, so it will not be necessary to provide it during the call.

The modeling of the class that allows the control of this HMI is:

    class Toueris2Hmi : public  WireSlave {
      public:
        Toueris2Hmi (int hirqPin = -1, byte slaveAddress = HMI4DINBOX_SLAVE_ADDR);
        bool begin();
        WireLeds led;
        WireKeyboard keyb;
        WireBackLight backlight;
    };

There are 3 modules: WireLeds (`led`), WireKeyboard (` keyb`) and WireBackLight (`backlight`) as well as 2 functions: the` Toueris2Hmi()` constructor and the` begin()` function.

The implementation of the Toueris2Hmi class is very concise (file [Toueris2Hmi.cpp](https://github.com/epsilonrt/WireHmi/blob/master/src/Toueris2Hmi.cpp)) :

    #define LED_REG 0
    #define BACKLIGHT_REG 1
    #define KEY_REG 2

The constants above correspond to the I2C register numbers assigned to each function.

    #define LED_NB 5

corresponds to the numbers of leds.

    Toueris2Hmi::Toueris2Hmi (int hirqPin, byte slaveAddress) :
      WireSlave (slaveAddress),
      led (this, LED_REG, LED_NB),
      keyb (this, KEY_REG, hirqPin),
      backlight (this, BACKLIGHT_REG) {
    }

The constructor takes in parameter the pin number indicating that a key can be read and possibly the address of our HMI.
This constructor confines itself to making calls to the constructors of the original WireSlave class and to the constructors of the constituent classes the interface with the previously defined constants.

    bool Toueris2Hmi::begin() {
      
      return this->led.begin() && this->keyb.begin() && this->backlight.begin();
    }

The `begin()` function performs a call to the `begin()` functions of each of the composition classes and returns a Boolean value that corresponds to the logical AND of all returned values.

------
