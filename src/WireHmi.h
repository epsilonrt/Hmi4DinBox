/*
   Copyright (c) 2018-2023, Pascal JEAN aka epsilonrt
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef __WIRE_HMI_H__
#define __WIRE_HMI_H__

#include <Arduino.h>
#include <Wire.h>

// I2c Slave Class
class WireSlave {
  public:
    WireSlave (byte slaveAddress);
    bool write (byte dataAddress, const byte *values, byte len);
    bool write (byte dataAddress, byte value);
    bool write (const byte *values, byte len);
    bool write (byte value);
    byte read (byte dataAddress, byte *values, byte len);
    byte read (byte dataAddress);
    byte read (byte *values, byte len);
    byte read();
    inline byte address() const {
      return _address;
    }
    inline void setAddress (byte slaveAddress) {
      _address = slaveAddress;
    }
  private:
    byte _address;
};

// I2c Data Register Class
class WireRegister {
  public:
    enum Mode { Read = 1, Write = 2, ReadWrite = Read + Write };
    WireRegister (WireSlave * slave, byte regAddress, Mode mode = ReadWrite);
    bool begin();
    byte read();
    bool write (byte value);
  private:
    WireSlave * _slave;
    byte _reg;
    Mode _mode;
};

// Leds Class
class WireLeds : protected WireRegister {
  public:
    WireLeds (WireSlave * slave, byte regAddress, byte size);
    bool begin();
    byte readAll();
    bool writeAll (byte all = 0xFF);
    bool toggleAll (byte all = 0xFF);
    bool set (byte led);
    bool clear (byte led);
    bool toggle (byte led);
    bool get (byte led);
    inline byte size() const {
      return _size;
    }
    inline byte all() const {
      return _mask;
    }
  private:
    byte _size;
    byte _mask;
    byte _leds;
};

// Keyboard Class
class WireKeyboard : protected WireRegister {
  public:
    WireKeyboard (WireSlave * slave, byte regAddress, int hirqPin = -1);
    bool begin();
    bool available();
    inline byte key() const {
      return _key & ~0x80;
    }
    inline bool released() const {
      return (_key & 0x80) != 0;
    }
    inline bool pressed() const {
      return ! released();
    }
  private:
    int _hirq;
    byte _key;
};

// BackLight Class
class WireBackLight : public WireRegister {
  public:
    WireBackLight (WireSlave * slave, byte regAddress);
    bool begin();
};

/* ========================================================================== */
#endif /* __WIRE_HMI_H__ */
