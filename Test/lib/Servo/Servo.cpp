#include "Servo.hpp"
#include <cstdlib>

extern TIM_HandleTypeDef htim1;


Servo::Servo(TIM_HandleTypeDef* timer, uint32_t ch)
{
    htim = timer;
    channel = ch;
}

void Servo::init()
{
    HAL_TIM_PWM_Start(htim, channel);
	htim1.Init.Prescaler = 8-1;
	htim1.Init.Period = 20000-1;
	HAL_TIM_Base_Init(&htim1);
	HAL_TIM_PWM_Init(&htim1);
}

void Servo::start()
{
    HAL_TIM_PWM_Start(htim, channel);
}

void Servo::stop()
{
    HAL_TIM_PWM_Stop(htim, channel);
}

void Servo::setAngle(float angle)
{
    if(angle < 0) angle = 0;
    if(angle > 180) angle = 180;

    uint16_t pulse = minPulse + (angle / 180.0f) * (maxPulse - minPulse);

    __HAL_TIM_SET_COMPARE(htim, channel, pulse);
}

void Servo::setSpeed(int speed)
{
    // speed = -100 to +100

    uint16_t pulse = 1500 + speed * 5;

    __HAL_TIM_SET_COMPARE(htim, channel, pulse);
}

void Servo::TestSweep(void)
{
    while (1)
    {
        // 0 → 180
        for(int angle = 0; angle <= 180; angle++)
        {
            setAngle(angle);
            HAL_Delay(20);
        }

        HAL_Delay(500);

        // 180 → 0
        for(int angle = 180; angle >= 0; angle--)
        {
            setAngle(angle);
            HAL_Delay(20);
        }

        HAL_Delay(500);
    }
}
