#include "Project_Helper.hpp"

#include <stdlib.h>
#include "Enum.hpp"
#include "CAN\can.hpp"
#include "Debug\Debug.h"
#include "Helper\Helper.hpp"


void printPacket(const Packet_t* pkt, Debug debug)
{
    if (pkt == nullptr) {
        debug.print("Packet: NULL\r\n");
        return;
    }

//    debug.print("\r\n========== PACKET DUMP ==========\r\n");
    debug.print("\r\n");
    debug.print("Valid      : %s\r\n", pkt->valid ? "TRUE" : "FALSE");
    debug.print("Length     : %u\r\n", pkt->length);
    debug.print("Trans Type : 0x%02X\r\n", pkt->transtype);
    debug.print("Cast       : %s\r\n", GetCastString(pkt->cast));
    debug.print("PIC Type   : %s\r\n", GetPIC_Type_String((PIC_Type_t)pkt->address));
    debug.print("Operation  : %s\r\n", GetRWString(pkt->rw));
    debug.print("Command    : %s\r\n", GetCommand_String(pkt->command));
    debug.print("Data Length: %u\r\n", pkt->dataLen);

    debug.print("Data       : ");

    for (uint8_t i = 0; i + 1 < pkt->dataLen; i += 2)
    {
        uint16_t value = ReadUInt16_BE(&pkt->data[i]);

        debug.print("%u", value);

        if (i < pkt->dataLen - 2)
            debug.print(",");
    }

    debug.print("\r\n");
    debug.print("CRC        : 0x%04X", pkt->crc);

//    debug.print("=================================\r\n\r\n");
}


const char* GetRWString(uint8_t rw)
{
    switch (rw)
    {
        case 0: return "READ";
        case 1: return "WRITE";
        case 2: return "EXECUTE";
        default: return "INVALID";
    }
}
//________________________________________________________________________________________________________________
const char* GetCastString(uint8_t cast)
{
    switch (cast)
    {
        case 0: return "UNICAST";
        case 1: return "MULTICAST";
        default: return "INVALID";
    }
}
//________________________________________________________________________________________________________________
const char* GetPIC_Type_String(PIC_Type_t pic_type)
{
    switch (pic_type)
    {
        case DISP_3X3_GRAINS: return "DISP_3X3_GRAINS";
        case DISP_3X3_LIQUID: return "DISP_3X3_LIQUID";
        case DISP_3X3_PUREE:  return "DISP_3X3_PUREE";
        case DISP_6X6_GRAINS: return "DISP_6X6_GRAINS";
        case DISP_6X6_LIQUID: return "DISP_6X6_LIQUID";
        case DISP_6X6_PUREE:  return "DISP_6X6_PUREE";

        case IP:          return "IP";
        case CHIMNEY:     return "CHIMNEY";
        case DISHWASHER:  return "DISHWASHER";
        case WTS:         return "WTS";

        case MOTOR_DRIVER_1: return "MOTOR_DRIVER_1";

        case MOTOR_X: return "MOTOR_X";
        case MOTOR_Y: return "MOTOR_Y";

        case MOTOR_R1: return "MOTOR_R1";
        case MOTOR_R2: return "MOTOR_R2";
        case MOTOR_R3: return "MOTOR_R3";
        case MOTOR_R4: return "MOTOR_R4";
        case MOTOR_R5: return "MOTOR_R5";

        case MOTOR_L1: return "MOTOR_L1";
        case MOTOR_L2: return "MOTOR_L2";
        case MOTOR_L3: return "MOTOR_L3";
        case MOTOR_L4: return "MOTOR_L4";

        default: return "INVALID";
    }
}
//________________________________________________________________________________________________________________
const char* GetCommand_String(uint16_t cmd)
{
    switch (cmd)
    {
        case 0: return "Who_Are_You";
        case 1: return "Dispense_Timer_Based";
        case 2: return "Dispense_Weight_Based";
        case 3: return "LED_TOGGLING_1";
        case 4: return "LED_TOGGLING_2";

        default: return "INVALID Command";
    }
}
//________________________________________________________________________________________________________________

