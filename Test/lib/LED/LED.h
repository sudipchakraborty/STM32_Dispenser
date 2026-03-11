#ifndef LED_H
#define LED_H

#include "main.h" // Needed for HAL types like GPIO_TypeDef

class LED {
public:
    // Constructor: Pass the port and pin (e.g., GPIOB, GPIO_PIN_5)
    LED(GPIO_TypeDef* port, uint16_t pin);

    void On();
    void Off();
    void Toggle();

private:
    GPIO_TypeDef* _port;
    uint16_t _pin;
};

#endif // LED_H
