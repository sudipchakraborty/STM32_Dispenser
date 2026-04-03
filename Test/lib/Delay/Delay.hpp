#ifndef DELAY_HPP
#define DELAY_HPP

#include "stm32f0xx_hal.h"

class Delay
{
public:
    static void ms(uint32_t ms);
    static void us(uint16_t us);
};

#endif
