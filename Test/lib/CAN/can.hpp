#ifndef CAN_HPP
#define CAN_HPP

#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>   // for C

#define KBUS_MAX_PACKET 128

#define PREAMBLE_SIZE 2
#define LENGTH_SIZE   1
#define CRC_SIZE      2
#define POSTAMBLE_SIZE 2

typedef struct
{
    bool valid;

    uint8_t length;
    uint8_t transtype;
    uint8_t cast;

    uint16_t address;
    uint8_t rw;
    uint8_t command;

    uint8_t data[64];
    uint8_t dataLen;

    uint16_t crc;

} Packet_t;
////////////////////////////////
typedef enum {
	source_buffer_destination_empty=0,
	packet_length_minium=1,
	preambles_not_found=2,
	postambles_not_found=3,
	packet_length_exceeds=4,
	CRC_error=5

}Error;


typedef enum
{
    read = 0,
    write = 1,
    execute = 2
} Operation;
////////////////////////////////

#ifdef __cplusplus

class CANProtocol
{
public:
    bool ParseError(uint8_t* buffer, uint16_t len, Packet_t* pkt);
    int Get_Sample_Dispense_Packet(uint8_t* buffer);
    unsigned short crc16_xmodem(const unsigned char *data, int len);
    uint16_t BuildPacket(const Packet_t* pkt, uint8_t* buffer);
    uint16_t Get_Demo_Packet(uint8_t* buffer);
    void Demo_Process();
};

#endif

#endif
