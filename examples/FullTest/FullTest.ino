// Hmi4DinBox Class and WireLcd Library Test
// by epsilonrt <https://github.com/epsilonrt>

// Control and display of the Hmi4DinBox

// Created 23 January 2018

// This example code is in the public domain.

#include <Hmi4DinBox.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <avr/wdt.h>

//------------------------------------------------------------------------------
const unsigned pause = 1000;
const int hirqPin = 7;
const int ledPin = 13;

Hmi4DinBox hmi (hirqPin);
char buffer[32];
byte index = 0;

void setup() {
  int loops = 0;

  MCUSR = 0;
  wdt_disable();

  Serial.begin (38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println ("Hmi4DinBox Class Test");
  Serial.println ("Available commands:");
  Serial.println (" bXXX:\t to set backlight to XXX (0-255)");
  Serial.println (" sX:\t to turn on led X (0-4)");
  Serial.println (" cX:\t to turn off led X (0-4)");
  Serial.println (" tX:\t to toggle led X (0-4)");
  Serial.println (" rX:\t to read led X (0-4)");
  Serial.println (" WXX:\t write all leds (0-1F)");
  Serial.println (" TXX:\t toggle all leds (0-1F)");
  Serial.println (" R:\t read all leds");
  Serial.println (" p:\t print text on LCD");
  Serial.println (" e:\t erase LCD");
  Serial.println (" gXX,YY: LCD goto to X,Y");
  Serial.println (" K:\t RESET by watchdog");

  // The LED is lit while waiting for the slave HMI
  pinMode (ledPin, OUTPUT);
  digitalWrite (ledPin, 1);

  while (!hmi.begin (24)) {
    loops++; // One waiting loop per second
    Serial.println ("hmi.begin() failed ! check the connections to the HMI.");
    delay (1000);
  }

  hmi.lcd.cursor();
  hmi.lcd.blink();

  digitalWrite (ledPin, 0);

  // The led flashes to inform the number of waiting loops
  for (byte j = 0; j < loops; j++) {

    digitalWrite (ledPin, 1);
    delay (200);
    digitalWrite (ledPin, 0);
    delay (200);
  }
}

void verify (int i) {
  if (i) {
    Serial.println ("\n>Ok");
  }
  else {
    Serial.println ("\n>Fail");
  }
}

bool test (int i) {
  if (!i) {
    Serial.println ("\n>Fail");
    return false;
  }
  return true;
}


bool str2byte (const char * str, byte * value, byte base = 0) {
  char *endptr;
  long val;

  errno = 0;    /* Pour distinguer la réussite/échec après l'appel */
  val = strtol (str, &endptr, base);

  if ( (errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
       || (errno != 0 && val == 0)) {

    return false;
  }

  if ( (endptr == str) || (val > 255) || (val < 0)) {

    return false;
  }
  *value = (byte) val;
  return true;
}

void loop() {

  if (Serial.available() > 0) {
    // read the incoming byte:
    char c = Serial.read();
    if ( ( (c != '\n') && (c != '\r')) && (index < (sizeof (buffer) - 1))) {
      buffer[index++] = c;
    }
    else {
      byte value, read_value;
      bool success;

      buffer[index] = 0;
      switch (buffer[0]) {
        case 'K':
        case 'k':
          buffer[0] = 0;
          index = 0;
          wdt_enable (WDTO_15MS);
          for (;;);
          break;
        case 'b':
          if (str2byte (&buffer[1], &value)) {
            success = hmi.backlight.write (value);
            read_value = hmi.backlight.read();
            verify (success & (read_value == value));
          }
          else {
            verify (false);
          }
          break;
        case 's':
          if (str2byte (&buffer[1], &value)) {

            if (test ( (value >= 0) && (value <= hmi.led.size()))) {

              success = hmi.led.set (value);
              read_value = hmi.led.get (value);
              verify (success & (read_value == true));
            }
            else {
              verify (false);
            }
          }
          else {
            verify (false);
          }
          break;
        case 'c':
          if (str2byte (&buffer[1], &value)) {

            if (test ( (value >= 0) && (value <= hmi.led.size()))) {

              success = hmi.led.clear (value);
              read_value = hmi.led.get (value);
              verify (success & (read_value == false));
            }
            else {
              verify (false);
            }
          }
          else {
            verify (false);
          }
          break;
        case 't':
          if (str2byte (&buffer[1], &value)) {

            if (test ( (value >= 0) && (value <= hmi.led.size()))) {

              success = hmi.led.toggle (value);
              verify (success);
            }
            else {
              verify (false);
            }
          }
          else {
            verify (false);
          }
          break;
        case 'T':
          if (str2byte (&buffer[1], &value, 16)) {

            if (test ( (value >= 0) && (value <=  hmi.led.all()))) {

              success = hmi.led.toggleAll (value);
              verify (success);
            }
            else {
              verify (false);
            }
          }
          else {
            verify (false);
          }
          break;
        case 'W':
          if (str2byte (&buffer[1], &value, 16)) {

            if (test ( (value >= 0) && (value <= hmi.led.all()))) {

              success = hmi.led.writeAll (value);
              read_value = hmi.led.readAll ();
              verify (success & (read_value == value));
            }
            else {
              verify (false);
            }
          }
          else {
            verify (false);
          }
          break;
        case 'R':
          read_value = hmi.led.readAll ();
          Serial.print ("\n>");
          Serial.println (read_value, HEX);
          break;
        case 'p':
          hmi.lcd.print (&buffer[1]);
          break;
        case 'e':
          hmi.lcd.clear();
          break;
        case 'g': {
          int comma;
          String s (&buffer[1]);
          comma = s.indexOf (',');
          if (comma >= 1) {
            String sx = s.substring (1, comma - 1);
            String sy = s.substring (comma + 1, s.length());
            Serial.print ("goto(");
            Serial.print (sx);
            Serial.print (",");
            Serial.print (sy);
            Serial.println (")");
            hmi.lcd.setCursor (sx.toInt(), sy.toInt());
          }
        }
        break;
        case 'r':
          if (str2byte (&buffer[1], &value)) {

            if (test ( (value >= 0) && (value <= hmi.led.size()))) {

              read_value = hmi.led.get (value);
              if (read_value) {
                Serial.println ("\n>1");
              }
              else {
                Serial.println ("\n>0");
              }
            }
            else {
              verify (false);
            }
          }
          else {
            verify (false);
          }
          break;
        default:
          Serial.println ("\n>Invalid command !");
          break;
      }
      buffer[0] = 0;
      index = 0;
    }
  }

  if (hmi.keyb.available()) {
    byte key = hmi.keyb.key();

    if (hmi.keyb.released()) {
      Serial.write ('R');
    }
    else {

      Serial.write ('P');
    }
    Serial.println (key);
  }
}
//------------------------------------------------------------------------------
