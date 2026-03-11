#include "HX711.h"
#include <stdlib.h>

HX711::HX711(GPIO_TypeDef* dout_port, uint16_t dout_pin,
             GPIO_TypeDef* sck_port, uint16_t sck_pin)
    : _dout_port(dout_port), _dout_pin(dout_pin),
      _sck_port(sck_port), _sck_pin(sck_pin),
      _index(0), _runningSum(0) {

    for(int i = 0; i < BUFFER_SIZE; i++) {
        _buffer[i] = OFFSET;
        _runningSum += OFFSET;
    }
}

void HX711::Init() {
    // GPIO Init is handled in MX_GPIO_Init, but we ensure SCK starts low
    HAL_GPIO_WritePin(_sck_port, _sck_pin, GPIO_PIN_RESET);
}

bool HX711::ReadRaw(long &value) {
    long count = 0;
    uint32_t timeout = HAL_GetTick();

    // Wait for DOUT to go low (Data Ready)
    while (HAL_GPIO_ReadPin(_dout_port, _dout_pin)) {
        if ((HAL_GetTick() - timeout) > 100) return false;
    }

    // Pulse SCK 24 times to read 24-bit data
    for (int i = 0; i < 24; i++) {
        HAL_GPIO_WritePin(_sck_port, _sck_pin, GPIO_PIN_SET);
        count <<= 1;
        HAL_GPIO_WritePin(_sck_port, _sck_pin, GPIO_PIN_RESET);

        if (HAL_GPIO_ReadPin(_dout_port, _dout_pin)) count++;
    }

    // 25th pulse sets gain to 128 for next read
    HAL_GPIO_WritePin(_sck_port, _sck_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(_sck_port, _sck_pin, GPIO_PIN_RESET);

    // Sign extend for 24-bit negative numbers
    if (count & 0x800000) count |= ~0xFFFFFF;

    value = count;
    return true;
}

long HX711::GetWeightX100() {
    long raw;
    if (!ReadRaw(raw)) return -99999; // Error code

    // Update Moving Average
    _runningSum -= _buffer[_index];
    _buffer[_index] = raw;
    _runningSum += raw;
    _index = (_index + 1) % BUFFER_SIZE;

    long average = _runningSum / BUFFER_SIZE;
    long weight_x100 = ((average - OFFSET) * 100L) / SCALE;

    // Dead zone ±2g
    if (weight_x100 < 200 && weight_x100 > -200) weight_x100 = 0;

    return weight_x100;
}
