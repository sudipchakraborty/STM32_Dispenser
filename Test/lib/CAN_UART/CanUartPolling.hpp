#ifndef CANUART_POLLING_HPP
#define CANUART_POLLING_HPP

#include "main.h"
#include <stdint.h>

#define KBUS_MAX_PACKET 128

class CanUartPolling
{
public:
    CanUartPolling(UART_HandleTypeDef* huart);

    void Process();   // Call inside main loop

    bool IsPacketReady();
    uint8_t* GetPacket();
    uint8_t GetPacketLength();
    bool SendPacket(uint8_t* buffer);

private:
    UART_HandleTypeDef* _huart;

    uint8_t rxBuffer[KBUS_MAX_PACKET];
    uint8_t rxIndex = 0;
    uint8_t expectedLength = 0;
    bool packetReady = false;
};

#endif
