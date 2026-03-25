#ifndef CAN_HPP
#define CAN_HPP

#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>   // for C

#define KBUS_MAX_PACKET 128

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
    bool Parse(uint8_t* buffer, uint16_t len, Packet_t* pkt);
};

#endif

#endif
