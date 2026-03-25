#include "CanUartPolling.hpp"
#include <string.h>
/////////////////////////////////////////////////////////
CanUartPolling::CanUartPolling(UART_HandleTypeDef* huart)
{
    _huart = huart;
}
//_____________________________________________________________________________________________________________________
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
//_____________________________________________________________________________________________________________________
bool CanUartPolling::received()
{
    uint8_t byte;

    // Try to receive 1 byte (non-blocking)
    if (HAL_UART_Receive(_huart, &byte, 1, 1) == HAL_OK)
    {
        // Mark reception active
        rxActive = true;
        lastRxTime = HAL_GetTick();   // update last receive time

        // Store data
        if (rxIndex < KBUS_MAX_PACKET)
        {
            rxBuffer[rxIndex++] = byte;
        }
        else
        {
            rxIndex = 0; // overflow protection
        }
    }

    // 🔥 Check timeout condition (10 ms)
    if (rxActive)
    {
        if ((HAL_GetTick() - lastRxTime) > 10)
        {
            // Reception finished
            rxActive = false;
            return true;   // ✅ Data reception completed
        }
    }
    return false;  // ❌ Not finished yet
}
//_____________________________________________________________________________________________________________________
uint16_t CanUartPolling::ConvertAsciiToHex(uint8_t* outBuf)
{
    uint16_t outIndex = 0;
    uint8_t highNibble = 0xFF;

    for (uint16_t i = 0; i < rxIndex; i++)
    {
        uint8_t c = rxBuffer[i];

        // Skip spaces
        if (c == ' ') continue;

        uint8_t nibble = HexCharToNibble(c);
        if (nibble == 0xFF) continue; // skip invalid

        if (highNibble == 0xFF)
        {
            highNibble = nibble; // store first half
        }
        else
        {
            // Combine two nibbles → 1 byte
            outBuf[outIndex++] = (highNibble << 4) | nibble;
            highNibble = 0xFF;
        }
    }

    return outIndex; // number of bytes generated
}
//_____________________________________________________________________________________________________________________
uint8_t CanUartPolling::HexCharToNibble(uint8_t c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0xFF; // invalid
}
//_____________________________________________________________________________________________________________________
bool CanUartPolling::reset()
{
	rxIndex = 0;
}
//_____________________________________________________________________________________________________________________
bool CanUartPolling::IsPacketReady()
{
    return packetReady;
}
//_____________________________________________________________________________________________________________________
uint8_t* CanUartPolling::GetPacket()
{
    packetReady = false;
    return rxBuffer;
}
//_____________________________________________________________________________________________________________________
uint8_t CanUartPolling::GetPacketLength()
{
    return rxBuffer[2];
}
//_____________________________________________________________________________________________________________________
bool CanUartPolling::SendPacket(uint8_t* buffer)
{
    uint8_t len = buffer[2];   // Packet length field

    if (len == 0 || len > KBUS_MAX_PACKET)
        return false;

    if (HAL_UART_Transmit(_huart, buffer, len, 100) == HAL_OK)
        return true;

    return false;
}
//_____________________________________________________________________________________________________________________
bool CanUartPolling::TestSend()
{
    // Test packet (your provided stream)
    uint8_t packet[] = {
        0x66, 0x55, 0x15, 0x00, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x0B, 0x36, 0x32, 0x03, 0x44, 0x03, 0x03,
        0x1A, 0x8F, 0x70, 0x77, 0x88
    };

    return SendPacket(packet);
}
//_____________________________________________________________________________________________________________________

