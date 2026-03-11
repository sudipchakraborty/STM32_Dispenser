#ifndef SERVO_HPP
#define SERVO_HPP

#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_tim.h"
#include <stdint.h>

class Servo
{
private:

    TIM_HandleTypeDef* htim;
    uint32_t channel;

    uint16_t minPulse = 1000;
    uint16_t maxPulse = 2000;

public:

    Servo(TIM_HandleTypeDef* timer, uint32_t ch);

    void init();
    void start();
    void stop();

    void setAngle(float angle);
    void setSpeed(int speed);
    void TestSweep(void);
};

#endif
