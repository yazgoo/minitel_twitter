MAIN=minitel
SERIAL_DEVICE=/dev/ttyACM0
DEPENDENCIES=wiring_analog.c WInterrupts.c wiring_digital.c \
    wiring_shift.c wiring_pulse.c wiring.c IPAddress.cpp \
    Stream.cpp Tone.cpp main.cpp CDC.cpp WMath.cpp \
    WString.cpp Print.cpp USBCore.cpp new.cpp HID.cpp \
    HardwareSerial.cpp
LIBRARIES=SoftwareSerial_SoftwareSerial
include /home/yazgoo/Dévelopement/arduino/arduino_makefile/Makefile
