#include "CanUartPolling.hpp"
#include <string.h>

CanUartPolling::CanUartPolling(UART_HandleTypeDef* huart)
{
    _huart = huart;
}

void CanUartPolling::Process()
{
    uint8_t byte;

    // Try to receive 1 byte (non-blocking)
    if (HAL_UART_Receive(_huart, &byte, 1, 1) == HAL_OK)
    {
        // 🔥 LED debug (remove later)
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);

        if (packetReady)
            return;

        rxBuffer[rxIndex++] = byte;

        // Validate header
        if (rxIndex == 2)
        {
            if (rxBuffer[0] != 0x66 || rxBuffer[1] != 0x55)
            {
                rxIndex = 0;
                return;
            }
        }

        // Third byte = total length
        if (rxIndex == 3)
        {
            expectedLength = rxBuffer[2];

            if (expectedLength < 3 || expectedLength > KBUS_MAX_PACKET)
            {
                rxIndex = 0;
                expectedLength = 0;
                return;
            }
        }

        if (expectedLength > 0 && rxIndex >= expectedLength)
        {
            packetReady = true;
            rxIndex = 0;
            expectedLength = 0;
        }
    }
}

bool CanUartPolling::IsPacketReady()
{
    return packetReady;
}

uint8_t* CanUartPolling::GetPacket()
{
    packetReady = false;
    return rxBuffer;
}

uint8_t CanUartPolling::GetPacketLength()
{
    return rxBuffer[2];
}

bool CanUartPolling::SendPacket(uint8_t* buffer)
{
    uint8_t len = buffer[2];   // Packet length field

    if (len == 0 || len > KBUS_MAX_PACKET)
        return false;

    if (HAL_UART_Transmit(_huart, buffer, len, 100) == HAL_OK)
        return true;

    return false;
}
