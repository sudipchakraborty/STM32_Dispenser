#include "Debug.h"
#include <stdio.h>
/////////////////////////////
Debug::Debug(UART_HandleTypeDef* huart) : _huart(huart) {}
//_______________________________________________________________________________________________________________
void Debug::print(const char* format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);

    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (len > 0) {
        HAL_UART_Transmit(_huart, (uint8_t*)buffer, len, 100);
    }
}
//_______________________________________________________________________________________________________________
// 👇 Test function
void Debug::test() {
    counter++;
    print("Debug Count: %lu\r\n", counter);
}
//_______________________________________________________________________________________________________________


