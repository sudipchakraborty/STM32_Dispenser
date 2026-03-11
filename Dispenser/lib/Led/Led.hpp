#pragma once

extern "C" {
#include "stm32f0xx_hal.h"
}

/*
 * Lightweight STM32 LED driver
 * Reusable across projects
 */

class Led
{
public:
    enum class State
    {
        Off = 0,
        On
    };

    Led(GPIO_TypeDef* port, uint16_t pin, bool activeHigh = true);

    void on();
    void off();
    void toggle();
    void set(State state);

    bool isOn() const;

private:
    GPIO_TypeDef* port_;
    uint16_t      pin_;
    bool          activeHigh_;
    bool          state_;
};
