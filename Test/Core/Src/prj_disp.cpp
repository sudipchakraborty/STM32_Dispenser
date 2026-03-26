#include <stdlib.h>
#include "main.h"
#include "LED/LED.h"
#include "Debug/Debug.h"
#include <cstdlib>
#include "CAN_UART/CanUartPolling.hpp"
#include "prj_disp.hpp"
#include "Servo/Servo.hpp"
#include "HX711/HX711.h"
#include "CAN/can.hpp"
#include "Helper/Helper.hpp"
///////////////////////////////////////////////////
// user name: sudipchakraborty
//Repo Token: "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"

//STM32 cube store password:
//--------------------------
//Question 1: module name
//Answer: Dispenser
//
//Question 2: product name
//Answer: Dispenser Kitchen Robot
///////////////////////////////////////////////////
// UART handles
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

// Polling CAN object
CanUartPolling can(&huart1);
CANProtocol canP;

// Servo motor
extern TIM_HandleTypeDef htim1;
Servo servo1(&htim1, TIM_CHANNEL_1);

// Debug object
static Debug debug(&huart2);

// LEDs
static LED led1(GPIOB, GPIO_PIN_5);
static LED led2(GPIOB, GPIO_PIN_6);

// HX711 instance
HX711 hx(GPIOB, GPIO_PIN_11,   // DOUT
         GPIOB, GPIO_PIN_10);  // SCK
//////////////////////////////////////////////
extern "C" void prj_Disp_init(void)
{
	hx.Init();
	servo1.init();
    debug.print("--- System Started (Polling Mode) ---\r\n");
    servo1.start();
    hx.Init();
    debug.print("HX711 Initialized\r\n");
    servo1.close();
    ////////////////
    Packet_t pkt = {0};

    pkt.transtype = 0x00;
    pkt.cast = 0x01;
    pkt.address = 0x6D;
    pkt.rw = 0x02; // Execute
    pkt.command = 0x00;

    pkt.data[0] = 0x00;
    pkt.data[1] = 0x64;  // 200
    pkt.dataLen = 2;

    uint8_t buffer[64];

    uint16_t len = canP.BuildPacket(&pkt, buffer);




	Demo_Process();

    while(1)
	{
//		process_Real_Hardware();


	}
}
//______________________________________________________________________________________________________________________
void Demo_Process()
{
	uint8_t bfr[100];

	int len=canP.Get_Sample_Dispense_Packet(bfr);
	debug.print("Demo Packet Received..\r\n");

	Packet_t pkt;

	if(canP.ParseError(bfr, len, &pkt)){
	}
	else
	{
		if(pkt.address==MyAddress)
		{
			process_command(pkt);
		}
	}
}
//______________________________________________________________________________________________________________________
void process_Real_Hardware()
{
	long value;
	uint8_t binaryBuffer[64];

	if(can.received())
		{
			int len=can.ConvertAsciiToHex(binaryBuffer);
			Packet_t pkt;
			if(canP.ParseError(binaryBuffer, len, &pkt))
			{
				if(pkt.address==MyAddress)
				{
					process_command(pkt);
				}
				 led1.Toggle();
			}
			can.reset();
		}

}
//______________________________________________________________________________________________________________________
void process_command(Packet_t pkt)
{
	uint16_t val;

	switch(pkt.rw)
	{
	case Operation::read:

		break;
	////////////////////////
	case Operation::write:

		break;
	///////////////////////
	case Operation::execute:
		val=ReadUInt16_BE(pkt.data);
		DispenseWeight(val, 60);
		break;
	///////////////////////
	default:
		break;
	//////////////////////
	}
}
//______________________________________________________________________________________________________________________
char DispenseWeight(long target_grams, int OpenInDeg)
{
    long startWeight = 0;
    long currentWeight = 0;

    // Read initial weight
    debug.print("Dispense Weight: %ld\r\n",target_grams);

    hx.GetWeight(startWeight);
	debug.print("Initial Weight: %ld\r\n",startWeight);

    long targetFinal = startWeight - target_grams;
    debug.print("Target Weight Value: %ld\r\n",targetFinal);

    // Open valve
    servo1.open(OpenInDeg);
    debug.print("Valve Open(Deg.): %ld\r\n",OpenInDeg);

	while (1)
	{
		hx.GetWeight(currentWeight);
		debug.print("current Weight: %ld\r\n",currentWeight);

		// Stop slightly early to avoid overshoot
		if (currentWeight <= targetFinal)
		{
			servo1.close();
			debug.print(" Valve Closed");
			break;
		}
	}
 }
//______________________________________________________________________________________________________________________
