#ifndef DEBUG_H
#define DEBUG_H

#include "main.h" // main.h includes stm32f0xx_hal.h
#include <stdarg.h>
#include <stdio.h>

class Debug {
public:
    // If UART_HandleTypeDef is still red, it means HAL_UART_MODULE_ENABLED
    // is definitely not defined in stm32f0xx_hal_conf.h
    Debug(UART_HandleTypeDef* huart);
    void print(const char* format, ...);

private:
    UART_HandleTypeDef* _huart;
};

#endif
