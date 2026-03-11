#include "Debug.h"

Debug::Debug(UART_HandleTypeDef* huart) : _huart(huart) {}

void Debug::print(const char* format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);

    // Format the string into the buffer
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (len > 0) {
        // Send via Polling mode (100ms timeout)
        HAL_UART_Transmit(_huart, (uint8_t*)buffer, len, 100);
    }
}
