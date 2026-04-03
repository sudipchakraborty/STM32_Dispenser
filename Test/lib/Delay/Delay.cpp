#include "Delay.hpp"

extern TIM_HandleTypeDef htim1; // Make sure this matches your timer

void Delay::ms(uint32_t ms)
{
    HAL_Delay(ms);
}

void Delay::us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);

    while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}
