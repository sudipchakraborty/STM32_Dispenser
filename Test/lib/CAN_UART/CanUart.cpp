#include "CanUart.hpp"
#include <string.h>

CanUart::CanUart(UART_HandleTypeDef* huart)
{
    _huart = huart;
}

void CanUart::Init()
{
    HAL_UART_Receive_IT(_huart, &rxByte, 1);
}

void CanUart::HandleInterrupt()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    ProcessRxByte(rxByte);
    HAL_UART_Receive_IT(_huart, &rxByte, 1);
}

bool CanUart::SendRaw(uint8_t* data, uint8_t length)
{
    if (data == nullptr || length == 0)
        return false;

    return (HAL_UART_Transmit(_huart, data, length, 100) == HAL_OK);
}

void CanUart::ProcessRxByte(uint8_t byte)
{
	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);   // Change pin if needed
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

    // Third byte = total packet length
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

bool CanUart::IsPacketReady()
{
    return packetReady;
}

uint8_t* CanUart::GetPacket()
{
    packetReady = false;
    return rxBuffer;
}

uint8_t CanUart::GetPacketLength()
{
    return rxBuffer[2];
}
