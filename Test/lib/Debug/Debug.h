#ifndef DEBUG_H
#define DEBUG_H

#include "main.h"
#include <stdarg.h>
#include <stdio.h>

class Debug {
public:
    Debug(UART_HandleTypeDef* huart);

    void print(const char* format, ...);
    void test();   // 👈 add this
private:
    UART_HandleTypeDef* _huart;
    uint32_t counter = 0;  // 👈 internal counter
};

#endif
