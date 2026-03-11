#pragma once

extern "C" {
#include "main.h"
}

class Hx711
{
public:
    Hx711(GPIO_TypeDef* dtPort,
          uint16_t dtPin,
          GPIO_TypeDef* sckPort,
          uint16_t sckPin,
          long scale,
          long offset);

    void init();
    bool readRaw(long &value);
    long getWeight_x100();   // returns weight * 100

private:
    GPIO_TypeDef* _dtPort;
    uint16_t _dtPin;

    GPIO_TypeDef* _sckPort;
    uint16_t _sckPin;

    long _scale;
    long _offset;

    static const uint8_t BUFFER_SIZE = 20;
    long _buffer[BUFFER_SIZE];
    uint8_t _index;
    long _runningSum;

    void movingAverage(long newValue);
};