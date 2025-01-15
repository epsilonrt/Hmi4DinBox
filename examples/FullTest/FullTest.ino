// Hmi4DinBox Class and WireLcd Library Test
// by epsilonrt <https://github.com/epsilonrt>

// Control and display of the Hmi4DinBox

// Created 23 January 2018

// This example code is in the public domain.
#ifdef __unix__
#include <Piduino.h>  // All the magic is here ;-)
#else
// Defines the serial port as the console on the Arduino platform
#define Console Serial
#endif

#include <Hmi4DinBox.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

const unsigned pause = 1000;
const int hirqPin = 7;

Hmi4DinBox hmi (hirqPin);
char buffer[32];
byte idx = 0;

void setup() {

  Console.begin (115200);
  Wire.begin(); // change this if you use another I2C port or if you use non standard I2C pins
  Console.println ("Hmi4DinBox Class Test");
  Console.println ("Available commands:");
  Console.println (" bXXX:\t to set backlight to XXX (0-255)");
  Console.println (" sX:\t to turn on led X (0-4)");
  Console.println (" cX:\t to turn off led X (0-4)");
  Console.println (" tX:\t to toggle led X (0-4)");
  Console.println (" rX:\t to read led X (0-4)");
  Console.println (" WXX:\t write all leds (0-1F)");
  Console.println (" TXX:\t toggle all leds (0-1F)");
  Console.println (" R:\t read all leds");
  Console.println (" p:\t print text on LCD");
  Console.println (" e:\t erase LCD");
  Console.println (" gXX,YY: LCD goto to X,Y");
  Console.println (" q:\t quit");

  if (!hmi.begin (34, true)) {

    Console.println ("hmi.begin() failed !");
    exit (1); // HMI failed to start !
  }

  hmi.lcd.cursor();
  hmi.lcd.blink();
}

void verify (int i) {
  if (i) {
    Console.println (">Ok");
  }
  else {
    Console.println (">Fail");
  }
}

bool test (int i) {
  if (!i) {
    Console.println (">Fail");
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

  if (Console.available() > 0) {
    // read the incoming byte:
    char c = Console.read();
    if ( ( (c != '\n') && (c != '\r')) && (idx < (sizeof (buffer) - 1))) {
      buffer[idx++] = c;
    }
    else {
      byte value, read_value;
      bool success;

      buffer[idx] = 0;
      switch (buffer[0]) {
        case 'q':
        case 'Q':
          exit (0);
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
          Console.print ("\n>");
          Console.println (read_value, HEX);
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
            Console.print ("goto(");
            Console.print (sx);
            Console.print (",");
            Console.print (sy);
            Console.println (")");
            hmi.lcd.setCursor (sx.toInt(), sy.toInt());
          }
        }
        break;
        case 'r':
          if (str2byte (&buffer[1], &value)) {

            if (test ( (value >= 0) && (value <= hmi.led.size()))) {

              read_value = hmi.led.get (value);
              if (read_value) {
                Console.println ("\n>1");
              }
              else {
                Console.println ("\n>0");
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
          Console.println ("\n>Invalid command !");
          break;
      }
      buffer[0] = 0;
      idx = 0;
    }
  }

  if (hmi.keyb.available()) {
    byte key = hmi.keyb.key();

    if (hmi.keyb.released()) {
      Console.write ('R');
    }
    else {

      Console.write ('P');
    }
    Console.println (key);
  }
}
