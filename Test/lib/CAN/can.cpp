#include "can.hpp"





bool CANProtocol::Parse(uint8_t* buffer, uint16_t len, Packet_t* pkt)
{
    if (!buffer || !pkt) return false;

    // Minimum packet size check
    if (len < 12) return false;

    // 🔹 Check preamble
    if (buffer[0] != 0x66 || buffer[1] != 0x55)
        return false;

    uint8_t packetLen = buffer[2];

    // 🔹 Length validation
    if (packetLen != len || packetLen > KBUS_MAX_PACKET)
        return false;

    pkt->length = buffer[2];
    pkt->transtype = buffer[3];
    pkt->cast = buffer[4];

    pkt->address = (buffer[5] << 8) | buffer[6];

    pkt->rw = buffer[7];
    pkt->command = buffer[8];

    uint16_t index = 9;

    // 🔹 Data length calculation
    uint16_t dataLen = packetLen - (9 + 2 + 2); // header + CRC + postamble

    if (dataLen > sizeof(pkt->data))
        return false;

    pkt->dataLen = dataLen;

    for (uint16_t i = 0; i < dataLen; i++)
    {
        pkt->data[i] = buffer[index++];
    }

    // 🔹 CRC
    pkt->crc = (buffer[index] << 8) | buffer[index + 1];
    index += 2;

    // 🔹 Postamble check
    if (buffer[index] != 0x77 || buffer[index + 1] != 0x88)
        return false;

    pkt->valid = true;
    return true;
}
