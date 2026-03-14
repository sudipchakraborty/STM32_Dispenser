#include "main.h"
#include "LED/LED.h"
#include "Debug/Debug.h"
#include <cstdlib>
#include "CAN_UART/CanUartPolling.hpp"
#include "prj_disp.hpp"
#include "Servo/Servo.hpp"
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
//////////////////////////////////////////////
extern "C" void prj_Disp_init(void)
{
	servo1.init();
    debug.print("--- System Started (Polling Mode) ---\r\n");
    prj_Disp_loop();
}
///////////////////////////////////////////////
extern "C" void prj_Disp_loop(void)
{
    can.Process();

    if (can.IsPacketReady())
    {
        led1.Toggle();
        HAL_Delay(100);
        uint8_t* pkt = can.GetPacket();
        uint8_t len = can.GetPacketLength();

        debug.print("Packet Received. Length: %d\r\n", len);

        for (uint8_t i = 0; i < len; i++)
        {
            debug.print("%02X ", pkt[i]);
        }
        debug.print("\r\n");
    }

        led1.Toggle();


}
