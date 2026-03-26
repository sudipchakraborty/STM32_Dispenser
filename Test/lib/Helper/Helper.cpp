#include "Helper.hpp"
//__________________________________________________________________________________________________________________
uint16_t ReadUInt16_BE(const uint8_t* buffer)
{
    if (!buffer) return 0;

    return ((uint16_t)buffer[0] << 8) | buffer[1];
}
//________________________________________________________________________________________________________________
uint16_t ReadUInt16_LE(const uint8_t* buffer)
{
    if (!buffer) return 0;

    return ((uint16_t)buffer[1] << 8) | buffer[0];
}
//________________________________________________________________________________________________________________
// Big Endian (High byte first)
void WriteUInt16_BE(uint8_t* buffer, uint16_t value)
{
    if (!buffer) return;

    buffer[0] = (value >> 8) & 0xFF;  // High byte
    buffer[1] = value & 0xFF;         // Low byte
}

// Little Endian (Low byte first)
void WriteUInt16_LE(uint8_t* buffer, uint16_t value)
{
    if (!buffer) return;

    buffer[0] = value & 0xFF;         // Low byte
    buffer[1] = (value >> 8) & 0xFF;  // High byte
}
