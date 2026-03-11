#include "LED.h"

LED::LED(GPIO_TypeDef* port, uint16_t pin) : _port(port), _pin(pin) {
    // Initialization is handled by MX_GPIO_Init in main.c,
    // but we store the pin info here for control.
}

void LED::On() {
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_SET);
}

void LED::Off() {
    HAL_GPIO_WritePin(_port, _pin, GPIO_PIN_RESET);
}

void LED::Toggle() {
    HAL_GPIO_TogglePin(_port, _pin);
}
