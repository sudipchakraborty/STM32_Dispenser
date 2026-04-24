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
#include "Delay/delay.hpp"

#include "Project_Helper.hpp"
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

long Packet_count;
long value=0;

long HX711_ReadRaw(void);
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
    while(1)
	{

		process_Real_Hardware();
	}
}
//______________________________________________________________________________________________________________________

//__________________________________________________________________________________________
void process_Real_Hardware()
{
	long value;
	uint8_t binaryBuffer[64];
	char asciiBuffer[200];
	Packet_t pkt;

	if(can.received_CountBased())
		{
			Packet_count++;
			debug.print("\r\n=========== Packet: %03u ===========\r\n", Packet_count);
		 	uint16_t len = can.GetRawBuffer(binaryBuffer, sizeof(binaryBuffer));
		    BufferToAsciiString(binaryBuffer, len,asciiBuffer,sizeof(asciiBuffer));
		 	debug.print(asciiBuffer);
			printPacket(&pkt,debug);
			debug.print("\r\n==================================\r\n\r\n");


			if(canP.ParseError(binaryBuffer, len, &pkt))
			{
				debug.print("Packet Receive Error \r\n");
			}
			else
			{
				if(pkt.address==MyAddress)
				{
					debug.print("This is my Packet \r\n");
					Delay::ms(1000);   // 1000 ms = 1 second delay
					process_command(pkt);
				}
			}

			can.reset();
		}
	else
	{
		led1.Toggle();
	}
}
//______________________________________________________________________________________________________________________
void process_command(Packet_t pkt)
{
	uint16_t val;
	uint16_t temp, Previous_Weight, Current_Weight;

	uint16_t diff;

	switch(pkt.rw)
	{
	case Operation::read:

		break;
	////////////////////////
	case Operation::write:

//		if(pkt.command==2)	// Weight Based Dispense
//		{
//			value =HX711_ReadRaw();
//			Previous_Weight=hx.GetCalibratedWeight(value);
//			debug.print("Previous Weight: %d\r\n", Previous_Weight);
//			Delay::ms(500);
//
//			servo1.moveFast(90);
//			do
//			{
//				value =HX711_ReadRaw();
//				Current_Weight=hx.GetCalibratedWeight(value);
//				debug.print("current Weight: %d\r\n", Current_Weight);
//				Delay::ms(200);
//
//				diff=Previous_Weight-150;
//				debug.print("diff Weight: %d\r\n", diff);
//
//
//			}while(Current_Weight>diff);

//			servo1.fastClose();


//			return;



//			temp =HX711_ReadRaw();
			//    		value=hx.GetCalibratedWeight(value);
			//    		debug.print("ADC Value: %d\r\n", value);
			//    		Delay::ms(500);


//			debug.print("Triggered Dispense Based weight \r\n");
//			servo1.moveFast(90);
//			Delay::ms(3000);
//			servo1.fastClose();
//		}

		if(pkt.command==5)
		{
			servo1.moveFast(90);
			Delay::ms(3000);
			servo1.fastClose();
		}

		if(pkt.command==6)
		{
			servo1.fastClose();
		}

		break;
	///////////////////////
	case Operation::execute:

		if(pkt.command==5) // 5: return "Open_Valve,";
		{
			uint16_t values[64];
			size_t count = Packet_GetUint16Array(&pkt, values, 64);
			debug.print("Open command received:5 \r\n");
			debug.print("Rotation angle:%d \r\n",values[0]);
			servo1.moveFast(values[0]);
		}

		if(pkt.command==6) // 5: return "Open_Valve,";
		{
			debug.print("Close command received:6 \r\n");
			servo1.fastClose();
		}

		if(pkt.command==2) // "Dispense_Weight_Based";
		{
			debug.print("Dispense_Weight_Based command received:2 \r\n");
			val=ReadUInt16_BE(pkt.data);
			DispenseWeight(val, 90);
		}

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
    float startWeight = 0;
    float currentWeight = 0;
    float TimeOutCount=0;
    float WeightBackUp[5];
    int index=0;
    // Read initial weight
    debug.print("Dispense Weight: %ld\r\n",target_grams);

    startWeight=hx.GetWeight();
    debug.print("startWeight: %ld\r\n", (long)startWeight);
    Delay::ms(500);

    long targetFinal = startWeight - target_grams;
    debug.print("Target Weight Value: %ld\r\n",targetFinal);

   // Open valve
    servo1.open(OpenInDeg);
    debug.print("Valve Open(Deg.): %ld\r\n",OpenInDeg);

	while (1)
	{
		currentWeight=hx.GetWeight();
		debug.print("currentWeight: %ld\r\n", (long)currentWeight);
		Delay::ms(250);

		float currentWeight2=hx.GetWeight();
	    if(currentWeight==currentWeight2)
	    {
	    	debug.print("Error: Valve Opening Error..\r\n");
	    	return -1;
	    }

		////////////////////////////////
		if (currentWeight <= targetFinal)
		{
			servo1.close();
			debug.print(" Valve Closed");
			return 0;
		}
		////////////////////////////////
//		if(currentWeight==WeightBackUp)
//		{
//			TimeOutCount++;
//			debug.print("currentWeight==WeightBackUp..TimeOutCount..\r\n");
//			if(TimeOutCount>2)
//			{
//					debug.print("Error: Valve Opening Error..\r\n");
//					return -1;
//			}
//		}
//		else
//		{
//			WeightBackUp=currentWeight;
//			TimeOutCount=0;
//		}
		////////////////////////////////
	}
	return -1;
 }
//______________________________________________________________________________________________________________________
