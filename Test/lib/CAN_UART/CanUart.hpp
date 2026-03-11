#ifndef CANUART_HPP
#define CANUART_HPP

#include "main.h"
#include <stdint.h>

#define KBUS_MAX_PACKET 128

class CanUart
{
public:
    CanUart(UART_HandleTypeDef* huart);

    void Init();
    void HandleInterrupt();   // 🔥 NEW FUNCTION

    bool SendRaw(uint8_t* data, uint8_t length);

    bool IsPacketReady();
    uint8_t* GetPacket();
    uint8_t GetPacketLength();

private:
    void ProcessRxByte(uint8_t byte);

    UART_HandleTypeDef* _huart;

    uint8_t rxByte;

    uint8_t rxBuffer[KBUS_MAX_PACKET];
    uint8_t rxIndex = 0;
    uint8_t expectedLength = 0;

    bool packetReady = false;
};

#endif
