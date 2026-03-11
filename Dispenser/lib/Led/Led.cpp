#include "Led.hpp"

Led::Led(GPIO_TypeDef* port, uint16_t pin, bool activeHigh)
    : port_(port),
      pin_(pin),
      activeHigh_(activeHigh),
      state_(false)
{
}

void Led::on()
{
    HAL_GPIO_WritePin(
        port_,
        pin_,
        activeHigh_ ? GPIO_PIN_SET : GPIO_PIN_RESET
    );
    state_ = true;
}

void Led::off()
{
    HAL_GPIO_WritePin(
        port_,
        pin_,
        activeHigh_ ? GPIO_PIN_RESET : GPIO_PIN_SET
    );
    state_ = false;
}

void Led::toggle()
{
    HAL_GPIO_TogglePin(port_, pin_);
    state_ = !state_;
}

void Led::set(State state)
{
    if (state == State::On)
        on();
    else
        off();
}

bool Led::isOn() const
{
    return state_;
}
