#include "can.hpp"
#include "Debug/Debug.h"

extern UART_HandleTypeDef huart2;
extern Debug debug(&huart2);

//_________________________________________________________________________________________________________________________
bool CANProtocol::ParseError(uint8_t* buffer, uint16_t len, Packet_t* pkt)
{
    if (!buffer || !pkt) return source_buffer_destination_empty;

    // Minimum size: header + CRC + postamble
    if (len < 12) return packet_length_minium;

    // Preamble check
    if (buffer[0] != 0x66 || buffer[1] != 0x55) return preambles_not_found;

    // Postamble check
     if (buffer[len-2] != 0x77 || buffer[len -1] != 0x88) return postambles_not_found;

    uint8_t packetLen = buffer[2];
    // Optional strict length check (can relax later)
    if (packetLen != len || packetLen > KBUS_MAX_PACKET) return packet_length_exceeds;

    // 🔹 Fixed fields
    pkt->length    = buffer[2];
    pkt->transtype = buffer[3];
    pkt->cast      = buffer[4];
    pkt->address   = (buffer[5] << 8) | buffer[6];
    pkt->rw        = buffer[7];
    pkt->command   = buffer[8];

    uint16_t dataLen=len-13;
    pkt->dataLen = dataLen;

    for (uint16_t i = 0; i < pkt->dataLen; i++)
    {
	   pkt->data[i] = buffer[9 + i];
    }

    // 🔹 Extract CRC (Big-endian)
    uint16_t receivedCRC = (buffer[len-4] << 8) | buffer[len-3];
    pkt->crc = receivedCRC;

    // 🔥 CRC VALIDATION (CRC-16/XMODEM)
    unsigned short calcCRC=crc16_xmodem(buffer,9+dataLen);

    if (calcCRC != receivedCRC) return  CRC_error;

    pkt->valid = true;

    return false;
}
//_________________________________________________________________________________________________________________________
void CANProtocol::printPacket(const Packet_t* pkt)
{
    if (pkt == nullptr) {
        debug.print("Packet: NULL\r\n");
        return;
    }

    debug.print("\r\n========== PACKET DUMP ==========\r\n");

    debug.print("Valid      : %s\r\n", pkt->valid ? "TRUE" : "FALSE");
    debug.print("Length     : %u\r\n", pkt->length);
    debug.print("Trans Type : 0x%02X\r\n", pkt->transtype);
    debug.print("Cast       : 0x%02X\r\n", pkt->cast);

    debug.print("Address    : 0x%04X\r\n", pkt->address);
    debug.print("RW         : %s\r\n", pkt->rw ? "WRITE" : "READ");
    debug.print("Command    : 0x%02X\r\n", pkt->command);

    debug.print("Data Length: %u\r\n", pkt->dataLen);

    // Print data bytes
    debug.print("Data       : ");
    for (uint8_t i = 0; i < pkt->dataLen; i++) {
    	debug.print("%02X ", pkt->data[i]);
    }
    debug.print("\r\n");

    debug.print("CRC        : 0x%04X\r\n", pkt->crc);

    debug.print("=================================\r\n\r\n");
}
//_______________________________________________________________________________________________________________
int CANProtocol::Get_Sample_Dispense_Packet(uint8_t* buffer)
{
    if (!buffer) return 0;

    uint8_t sample[] =
    {
        0x66, 0x55, 	// preambles
		0x0F, 			// length
		0x00, 			// Transtype
		0x01, 			// Multicast
		0x00,0x6D,		// PIC address
		0x02,			// Execute
        0x00,			// command=Dispense
		0x00, 0x64, 	// Dispense amount= 100 gms.
		0xE2, 0x7E,		// CRC
		0x77, 0x88,		// postambles
    };

    // Copy to output buffer
    for (uint8_t i = 0; i < sizeof(sample); i++)
    {
        buffer[i] = sample[i];
    }
    return sizeof(sample);
}
//_________________________________________________________________________________________________________________________
unsigned short CANProtocol::crc16_xmodem(const unsigned char *data, int len) {
    unsigned short crc = 0x0000; // Initial value

    for (int i = 0; i < len; i++) {
        crc ^= (unsigned short)(data[i] << 8); // XOR byte into the high order byte of the CRC

        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021; // Shift left and XOR with the polynomial
            } else {
                crc <<= 1; // Just shift left
            }
        }
    }
    return crc;
}
//_________________________________________________________________________________________________________________________
uint16_t CANProtocol::BuildPacket(const Packet_t* pkt, uint8_t* buffer)
{
    if (!pkt || !buffer) return 0;

    uint16_t index = 0;

    // 🔹 Preamble
    buffer[index++] = 0x66;
    buffer[index++] = 0x55;

    // 🔹 Length placeholder (fill later)
    uint16_t lengthIndex = index++;

    // 🔹 Header
    buffer[index++] = pkt->transtype;
    buffer[index++] = pkt->cast;

    // 🔹 Address (Big Endian)
    buffer[index++] = (pkt->address >> 8) & 0xFF;
    buffer[index++] = pkt->address & 0xFF;

    buffer[index++] = pkt->rw;
    buffer[index++] = pkt->command;

    // 🔹 Data
    for (uint16_t i = 0; i < pkt->dataLen; i++)
    {
        buffer[index++] = pkt->data[i];
    }

    // 🔹 Calculate Length
    uint16_t totalLen = index + 2 + 2; // +CRC +postamble
    buffer[lengthIndex] = totalLen;

    // 🔥 CRC (XMODEM)
    uint16_t crc = crc16_xmodem(buffer, 9+ pkt->dataLen);

    // 🔹 Write CRC (Big Endian)
    buffer[index++] = (crc >> 8) & 0xFF;
    buffer[index++] = crc & 0xFF;

    // 🔹 Postamble
    buffer[index++] = 0x77;
    buffer[index++] = 0x88;

    return index; // total bytes
}
//_________________________________________________________________________________________________________________________
uint16_t CANProtocol::Get_Demo_Packet(uint8_t* buffer)
{
	Packet_t pkt = {0};

	pkt.transtype = 0x00;
	pkt.cast = 0x01;
	pkt.address = 0x6D;
	pkt.rw = 0x02; // Execute
	pkt.command = 0x00;

	pkt.data[0] = 0x00;
	pkt.data[1] = 0x64;  // 200
	pkt.dataLen = 2;

	uint16_t len = BuildPacket(&pkt, buffer);

	return len;
}
//_________________________________________________________________________________________________________________________
void CANProtocol::Demo_Process()
{
	uint8_t bfr[100];

	int len=Get_Sample_Dispense_Packet(bfr);

	Packet_t pkt;

	if(ParseError(bfr, len, &pkt)){
	}
	else
	{
		if(pkt.address==109)
		{
//			process_command(pkt);
		}
	}
}
//_________________________________________________________________________________________________________________________







