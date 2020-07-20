//
//    FILE: PCF8575.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 2020-07-20
// VERSION: 0.0.1
// PURPOSE: Arduino library for PCF8575 - 16 channel I2C IO expander
//     URL: https://github.com/RobTillaart/PCF8575
//
// HISTORY:
// 0.0.1   2020-07-20 initial version
//

#include "PCF8575.h"

PCF8575::PCF8575(const uint8_t deviceAddress)
{
    _address = deviceAddress;
    _dataIn = 0;
    _dataOut = 0xFFFF;
    _buttonMask = 0xFFFF;
    _error = PCF8575_OK;
}

#if defined (ESP8266) || defined(ESP32)
void PCF8575::begin(uint8_t sda, uint8_t scl, uint16_t val)
{
  Wire.begin(sda, scl);
  PCF8575::write16(val);
}
#endif

void PCF8575::begin(uint16_t val)
{
  Wire.begin();
  PCF8575::write16(val);
}

uint16_t PCF8575::read16()
{
  if (Wire.requestFrom(_address, (uint8_t)2) != 2)
  {
    _error = PCF8575_I2C_ERROR;
    return _dataIn; // last value
  }
  _dataIn = Wire.read() << 8;
  _dataIn |= Wire.read();
  return _dataIn;
}

void PCF8575::write16(const uint16_t value)
{
  _dataOut = value;
  Wire.beginTransmission(_address);
  Wire.write(_dataOut);
  Wire.write(_dataOut);
  _error = Wire.endTransmission();
}

uint8_t PCF8575::read(const uint8_t pin)
{
  if (pin > 15)
  {
    _error = PCF8575_PIN_ERROR;
    return 0;
  }
  PCF8575::read16();
  return (_dataIn & (1 << pin)) > 0;
}

void PCF8575::write(const uint8_t pin, const uint8_t value)
{
  if (pin > 15)
  {
    _error = PCF8575_PIN_ERROR;
    return;
  }
  if (value == LOW)
  {
    _dataOut &= ~(1 << pin);
  }
  else
  {
    _dataOut |= (1 << pin);
  }
  write16(_dataOut);
}

void PCF8575::toggle(const uint8_t pin)
{
  if (pin > 15)
  {
    _error = PCF8575_PIN_ERROR;
    return;
  }
  toggleMask(1 << pin);
}

void PCF8575::toggleMask(const uint16_t mask)
{
  _dataOut ^= mask;
  PCF8575::write16(_dataOut);
}

void PCF8575::shiftRight(const uint8_t n)
{
  if ((n == 0) || (_dataOut == 0)) return;
  if (n > 15) _dataOut = 0;          // shift 8++ clears all, valid...
  if (_dataOut != 0) _dataOut >>= n; // only shift if there are bits set
  PCF8575::write16(_dataOut);
}

void PCF8575::shiftLeft(const uint8_t n)
{
  if ((n == 0) || (_dataOut == 0)) return;
  if (n > 15) _dataOut = 0;           // shift 8++ clears all, valid...
  if (_dataOut != 0) _dataOut <<= n;  // only shift if there are bits set
  PCF8575::write16(_dataOut);
}

int PCF8575::lastError()
{
  int e = _error;
  _error = PCF8575_OK;
  return e;
}

void PCF8575::rotateRight(const uint8_t n)
{
  if ((n % 16) == 0) return;
  uint8_t r = n & 15;
  _dataOut = (_dataOut >> r) | (_dataOut << (15 - r));
  PCF8575::write16(_dataOut);
}

void PCF8575::rotateLeft(const uint8_t n)
{
  rotateRight(16- (n & 15));
}

void PCF8575::reverse() // quite fast
{
  uint8_t x = _dataOut;
  x = (((x & 0xaaaa) >> 1) | ((x & 0x5555) << 1));
  x = (((x & 0xcccc) >> 2) | ((x & 0x3333) << 2));
  x =            ((x >> 4) | (x << 4));
  x =            ((x >> 8) | (x << 8));
  PCF8575::write16(x);
}

//added 0.1.07/08 Septillion
uint16_t PCF8575::readButton16(const uint16_t mask)
{
  uint16_t temp = _dataOut;
  PCF8575::write16(mask | _dataOut); 
  PCF8575::read16();
  PCF8575::write16(temp);
  return _dataIn;
}

//added 0.1.07 Septillion
uint16_t PCF8575::readButton(const uint8_t pin)
{
  if (pin > 15)
  {
    _error = PCF8575_PIN_ERROR;
    return 0;
  }
  uint16_t temp = _dataOut;
  PCF8575::write(pin, HIGH);
  uint8_t rtn = PCF8575::read(pin);
  PCF8575::write16(temp);
  return rtn;
}

// -- END OF FILE --
