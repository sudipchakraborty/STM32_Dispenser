#ifndef MG996R_H
#define MG996R_H

#include "stm32f0xx_hal.h"

class MG996R
{
public:
    MG996R(TIM_HandleTypeDef* timer, uint32_t channel);

    void init();
    void start();
    void stop();

    void setAngle(float angle);
    void moveTo(float targetAngle, uint8_t step = 1, uint16_t delayMs = 20);
    void moveFast(float angle);
    void TestSweep(void);

    void open(float angle);
    void close();
    void fastClose();

    void setPulseRange(uint16_t min_us, uint16_t max_us); // calibration

private:
    TIM_HandleTypeDef* htim;
    uint32_t channel;

    float currentAngle = 0;

    // default MG996R safe values
//    uint16_t minPulse_us = 1000;  // 1 ms
//    uint16_t maxPulse_us = 2000;  // 2 ms

    uint16_t minPulse_us = 500;
    uint16_t maxPulse_us = 2500;

    uint32_t timerClockMHz = 1;   // depends on timer config

    uint32_t usToTicks(uint16_t us);
};

#endif
