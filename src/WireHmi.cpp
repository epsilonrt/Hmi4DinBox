/* Copyright © 2018 Pascal JEAN, All rights reserved.
 * This file is part of the WireHmi Library.
 *
 * The WireHmi Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * The WireHmi Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the WireHmi Library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "WireHmi.h"

// -----------------------------------------------------------------------------
//
//                            WireSlave class
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
WireSlave::WireSlave (byte slaveAddress) : _address (slaveAddress)  {
}

// -----------------------------------------------------------------------------
byte WireSlave::read (byte dataAddress, byte *values, byte len) {
  byte readBytes = 0;
  Wire.beginTransmission (_address); // transmit to device
  Wire.write (dataAddress);       // sends one byte
  Wire.endTransmission (false);   // repeat start transmitting
  readBytes += Wire.requestFrom (_address, len);   // request bytes from slave device
  while (Wire.available()) { // slave may send less than requested
    *values++ = Wire.read();    // receive a byte as character
  }
  return readBytes;
}

// -----------------------------------------------------------------------------
byte WireSlave::read (byte dataAddress) {
  byte data = 0xFF;
  this->read (dataAddress, &data, 1);
  return data;
}

// -----------------------------------------------------------------------------
byte WireSlave::read (byte *values, byte len) {
  byte readBytes = 0;
  Wire.beginTransmission (_address); // transmit to device
  readBytes += Wire.requestFrom (_address, len);   // request bytes from slave device
  while (Wire.available()) { // slave may send less than requested
    *values++ = Wire.read();    // receive a byte as character
  }
  return readBytes;
}

// -----------------------------------------------------------------------------
byte WireSlave::read () {
  byte data = 0xFF;
  this->read (&data, 1);
  return data;
}

// -----------------------------------------------------------------------------
bool WireSlave::write (byte dataAddress, const byte *values, byte len) {
  Wire.beginTransmission (_address); // transmit to device
  Wire.write (dataAddress);       // sends one byte
  Wire.write (values, len);  // sends data bytes
  return (Wire.endTransmission() == 0);    // stop transmitting
}

// -----------------------------------------------------------------------------
bool WireSlave::write (byte dataAddress, byte value) {
  return this->write (dataAddress, &value, 1);
}

// -----------------------------------------------------------------------------
bool WireSlave::write (const byte *values, byte len) {
  Wire.beginTransmission (_address); // transmit to device
  Wire.write (values, len);  // sends data bytes
  return (Wire.endTransmission() == 0);    // stop transmitting
}

// -----------------------------------------------------------------------------
bool WireSlave::write (byte value) {
  return this->write (&value, 1);
}

// -----------------------------------------------------------------------------
//
//                         WireRegister class
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
WireRegister::WireRegister (WireSlave * slave, byte regAddress, WireRegister::Mode mode) :
  _slave (slave), _reg (regAddress), _mode (mode) {

}

// -----------------------------------------------------------------------------
bool WireRegister::begin() {
  if (_mode & Read) {
    byte data, timeout = 10;
    byte count;

    while ( (timeout--) && (count = _slave->read (_reg, &data, 1))) {
      delay (100);;
    }
    return (count == 1);

  }
  return true;
}

// -----------------------------------------------------------------------------
byte WireRegister::read() {
  return (_mode & Read ? _slave->read (_reg) : 0);
}

// -----------------------------------------------------------------------------
bool WireRegister::write (byte value) {
  return (_mode & Write ? _slave->write (_reg, value) : false);
}

// -----------------------------------------------------------------------------
//
//                            WireLeds class
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
WireLeds::WireLeds (WireSlave * slave, byte regAddress, byte size) :
  WireRegister (slave, regAddress), _size (size), _mask ( (1 << size) - 1), _leds (0) {
}

// -----------------------------------------------------------------------------
bool WireLeds::begin() {

  if (WireRegister::begin()) {

    return this->write (_leds);
  }
  return false;
}

// -----------------------------------------------------------------------------
byte WireLeds::readAll() {

  _leds = this->read();
  return _leds;
}

// -----------------------------------------------------------------------------
bool WireLeds::writeAll (byte all) {

  _leds = all & _mask;
  return this->write (_leds);
}

// -----------------------------------------------------------------------------
bool WireLeds::toggleAll (byte all) {

  _leds ^= all & _mask;
  return this->write (_leds);
}

// -----------------------------------------------------------------------------
bool WireLeds::set (byte led) {
  byte b = (1 << led) & _mask;
  if (b) {
    _leds |= b;
    return this->write (_leds);
  }
  return false;
}

// -----------------------------------------------------------------------------
bool WireLeds::clear (byte led) {
  byte b = (1 << led) & _mask;
  if (b) {
    _leds &= ~b;
    return this->write (_leds);
  }
  return false;
}

// -----------------------------------------------------------------------------
bool WireLeds::toggle (byte led) {
  byte b = (1 << led) & _mask;
  if (b) {
    _leds ^= b;
    return this->write (_leds);
  }
  return false;
}

// -----------------------------------------------------------------------------
bool WireLeds::get (byte led) {
  byte b = (1 << led) & _mask;
  if (b) {
    _leds = this->read();
    return (_leds & b) != 0;
  }
  return false;
}

// -----------------------------------------------------------------------------
//
//                            WireKeyboard class
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
WireKeyboard::WireKeyboard (WireSlave * slave, byte regAddress, int hirqPin)  :
  WireRegister (slave, regAddress), _hirq (hirqPin), _key (0) {

}

// -----------------------------------------------------------------------------
bool WireKeyboard::begin() {

  if (WireRegister::begin()) {

    if (_hirq > 0) {
      pinMode (_hirq, INPUT);
    }
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
bool WireKeyboard::available() {
  if (_hirq > 0) {
    if (!digitalRead (_hirq)) {
      return false;
    }
  }
  _key = WireRegister::read();
  return _key != 0;
}

// -----------------------------------------------------------------------------
//
//                            WireBackLight class
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
WireBackLight::WireBackLight (WireSlave * slave, byte regAddress)  :
  WireRegister (slave, regAddress) {

}

// -----------------------------------------------------------------------------
bool WireBackLight::begin() {

  return WireRegister::begin();
}

/* ========================================================================== */
