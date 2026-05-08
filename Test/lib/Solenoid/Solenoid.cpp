#include "Solenoid.hpp"

Solenoid::Solenoid(GPIO_TypeDef* port, uint16_t pin)
{
    gpioPort = port;
    gpioPin = pin;
    state = false;

    off();
}

void Solenoid::on()
{
    HAL_GPIO_WritePin(gpioPort, gpioPin, GPIO_PIN_SET);
    state = true;
}

void Solenoid::off()
{
    HAL_GPIO_WritePin(gpioPort, gpioPin, GPIO_PIN_RESET);
    state = false;
}

void Solenoid::toggle()
{
    HAL_GPIO_TogglePin(gpioPort, gpioPin);

    state = !state;
}

void Solenoid::set(bool value)
{
    if(value)
    {
        on();
    }
    else
    {
        off();
    }
}

bool Solenoid::isOn()
{
    return state;
}
