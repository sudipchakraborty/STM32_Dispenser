#include "Debug.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>

static UART_HandleTypeDef huart2;

static void MX_USART2_UART_Init(uint32_t baudrate);

void Debug::init(uint32_t baudrate)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* PA2 -> TX , PA3 -> RX */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    MX_USART2_UART_Init(baudrate);
}

static void MX_USART2_UART_Init(uint32_t baudrate)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = baudrate;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

void Debug::print(const char* msg)
{
    HAL_UART_Transmit(&huart2,
                      (uint8_t*)msg,
                      strlen(msg),
                      HAL_MAX_DELAY);
}

void Debug::println(const char* msg)
{
    print(msg);
    print("\r\n");
}

void Debug::println(const char* label, long value)
{
    char buffer[64];

    long integerPart = value / 100;
    long decimalPart = labs(value % 100);

    snprintf(buffer,
             sizeof(buffer),
             "%s%ld.%02ld\r\n",
             label,
             integerPart,
             decimalPart);

    print(buffer);
}