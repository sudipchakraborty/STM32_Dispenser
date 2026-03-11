#include "Bios.hpp"

extern "C" {
#include "main.h"
}

extern "C" void SystemClock_Config(void);

/* Private GPIO init */
static void MX_GPIO_Init(void);

void Bios::init()
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
}

/* GPIO Init */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);
}
