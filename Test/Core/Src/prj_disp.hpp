#ifndef PRJ_DISP_HPP
#define PRJ_DISP_HPP
#include "CAN/can.hpp"

//#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
#define MyAddress 			101

#define PIC_DISP_3X3_GRAINS		101
#define PIC_DISP_3X3_LIQUID		102
#define PIC_DISP_3X3_PUREE		103
#define PIC_DISP_6X6_GRAINS		104
#define PIC_DISP_6X6_LIQUID		105
#define PIC_DISP_6X6_PUREE		106
#define PIC_REFRIGERATOR		11
#define PIC_GRINDER				16
////////////////////////////////////////
#define PIC_DISP_3X3_GRAINS_No	1
#define PIC_DISP_3X3_LIQUID_No	2
#define PIC_DISP_3X3_PUREE_No	3
#define PIC_DISP_6X6_GRAINS_No	4
#define PIC_DISP_6X6_LIQUID_No	5
#define PIC_DISP_6X6_PUREE_No	6
#define PIC_REFRIGERATOR_No		24
#define PIC_GRINDER_No			25
/////////////////////////////////////////


void prj_Disp_init(void);
void prj_Disp_loop(void);
char DispenseWeight(long target_grams, int OpenInDeg);
void process_command(Packet_t pkt);

#ifdef __cplusplus
}
#endif

#endif
