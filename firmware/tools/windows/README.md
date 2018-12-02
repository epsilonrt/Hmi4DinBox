# Upload firmware in Hmi4DinBox from Windows

		When running ArduinoSketchUploader.exe without arguments, the application will document its usage:

		ArduinoSketchUploader 3.2.0.0
		Copyright c Christophe Diericx 2016 - 2018

		ERROR(S):
			-f/--file required option is missing.
			-m/--model required option is missing.


			-f, --file     Required. Path to the input file (in intel HEX format) which
										 is to be uploaded to the Arduino.

			-p, --port     Name of the COM port where the Arduino is attached (e.g.
										 'COM1', 'COM2', 'COM3'...).

			-m, --model    Required. Arduino model. Valid parameters are any of the
										 following: [Leonardo, Mega1284, Mega2560, Micro, NanoR2,
										 NanoR3, UnoR3].

			--help         Display this help screen.

A sample command line invocation (for a Mega2560 type Arduino attached to COM4):

		ArduinoSketchUploader.exe --file=C:\MyHexFiles\myHexFile.hex --port=COM4 --model=Mega2560

If only a single COM port is in use on the system (used by the attached Arduino), one can omit the port:

		ArduinoSketchUploader.exe --file=C:\MyHexFiles\myHexFile.hex --model=UnoR3

The library has been tested with the following configurations:

		Arduino Model 	MCU 	Bootloader protocol
		Leonardo 	ATMega32U4 	AVR109
		Mega 1284 	ATMega1284 	STK500v1
		Mega 2560 	ATMega2560 	STK500v2
		Micro 	ATMega32U4 	AVR109
		Nano (R2) 	ATMega168 	STK500v1
		Nano (R3) 	ATMega328P 	STK500v1
		Uno (R3) 	ATMega328P 	STK500v1
