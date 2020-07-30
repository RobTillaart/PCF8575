# PCF8575

Arduino library for PCF8575 - 16 channel I2C IO expander

## Description

The library is based upon the PCF8574 library  https://github.com/RobTillaart/PCF8574

The library gives easy control over the 16 pins of the PCF8575 chips.

Base address = 0x20 + 0..7 depending on address pins A0..A2

| TYPE     | ADDRESS-RANGE | notes |
|:----|:----:|:----:|
|PCF8575   | 0x20 to 0x27 | same range as PCF8574 !! |
|       |     |    |

So you can connect up to 8 PCF8575 on one I2C bus, giving access 
to 8 x 16 = 128 IO lines. Be sure to have a well dimensioned power supply.

The library allows to read and write both single pins or 16 pins at once.
Furthermore some additional functions are implemented that are a little more
playfull but still are useful.

## I2C Clock

Testing showed that the PCF8575 still works at 600 KHz and failed at 800 KHz.
These values are outside the specs of the datasheet so they are not recommendend.
However when performance is neede you can try to overclock the chip. 

## Interface

**PCF8575_INITIAL_VALUE** is a define that can be set compile time or before
the include of "pcf8575.h" to overrule the default value used with the 
**begin()** call.

- **PCF8575(deviceAddress)** Constructor with I2C device address as parameter.
- **begin(val = PCF8575_INITIAL_VALUE)** set the initial value for the pins and masks.
- **begin(sda, scl, val = PCF8575_INITIAL_VALUE)** idem, for the ESP32 where one can choose the I2C pins
What needs to be added in the future is a parameter to choose another Wire interface
as some processors have multiple hardware Wire interfaces.
- **read16()** reads all 16 pins at once. This one does the actual reading.
- **read(pin)** reads a single pin; pin = 0..15
- **value()** returns the last read inputs again, as this information is buffered 
in the class this is faster than reread the pins.
- **write16(value)** writes all 16 pins at once. This one does the actual reading.
- **write(pin, value)** writes a single pin; pin = 0..15; value is HIGH(1) or LOW (0)
- **valueOut()** returns the last written data. 
- **setButtonMask(mask)** 
- **readButton16()**
- **readButton16(mask = 0xFFFF)**
- **readButton(pin)**
- **toggle(pin)** toggles a single pin
- **toggleMask(mask = 0xFFFF)** toggles a selection of pins, 
if you want to invert all pins use 0xFFFF (default value)
- **shiftRight(n = 1)** shifts output channels n pins (default 1) pins right (e.g. leds ). 
Fills the higher lines with zero's.
- **shiftLeft(n = 1)**  shifts output channels n pins (default 1) pins left (e.g. leds ).
Fills the lower lines with zero's.
- **rotateRight(n = 1)** rotates output channels to right, moving lowest line to highest line.
- **rotateLeft(n = 1)** rotates output channels to left, moving highest line to lowest line.
- **reverse()** revers the "bit pattern" of the lines, high to low and vice versa.

## Testing

Testing the initial library is done by Colin Mackay (thanks!).

Platforms used for testing include: Nano, ESP32 and Seeed Xiao


## Operation

See examples
