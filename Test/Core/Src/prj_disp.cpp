#include "main.h"
#include "LED/LED.h"
#include "Debug/Debug.h"
#include <cstdlib>
#include "CAN_UART/CanUartPolling.hpp"
#include "prj_disp.hpp"
#include "Servo/Servo.hpp"
#include "HX711/HX711.h"
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

    prj_Disp_loop();
}
///////////////////////////////////////////////
extern "C" void prj_Disp_loop(void)
{
	long value;

	while(1)
	{
		if(hx.GetWeight(value))
		{
			debug.print("ADC: %ld\r\n",value);
		}





//		servo1.setAngle(180);
//
//		servo1.open(90);   // open slowly to 120°
//		HAL_Delay(2000);
//
//		servo1.close();     // slowly go back to 0°
//		HAL_Delay(2000);

//    can.Process();
//
//    if (can.IsPacketReady())
//    {
//        led1.Toggle();
//        HAL_Delay(100);
//        uint8_t* pkt = can.GetPacket();
//        uint8_t len = can.GetPacketLength();
//
//        debug.print("Packet Received. Length: %d\r\n", len);
//
//        for (uint8_t i = 0; i < len; i++)
//        {
//            debug.print("%02X ", pkt[i]);
//        }
//        debug.print("\r\n");
//    }

        led1.Toggle();
//        HAL_Delay(10);
//        debug.print("--- System  Running.. ---\r\n");

	}
}

