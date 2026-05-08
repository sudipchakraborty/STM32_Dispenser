#ifndef SOLENOID_HPP
#define SOLENOID_HPP

#include "main.h"

class Solenoid
{
private:
    GPIO_TypeDef* gpioPort;
    uint16_t gpioPin;
    bool state;

public:
    Solenoid(GPIO_TypeDef* port, uint16_t pin);

    void on();
    void off();
    void toggle();
    void set(bool value);

    bool isOn();
};

#endif
