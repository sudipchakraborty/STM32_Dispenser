#include "Hx711.hpp"

static inline void short_delay()
{
    for (volatile int i = 0; i < 20; i++);  // small delay for HX711 timing
}

Hx711::Hx711(GPIO_TypeDef* dtPort,
             uint16_t dtPin,
             GPIO_TypeDef* sckPort,
             uint16_t sckPin,
             long scale,
             long offset)
    : _dtPort(dtPort),
      _dtPin(dtPin),
      _sckPort(sckPort),
      _sckPin(sckPin),
      _scale(scale),
      _offset(offset),
      _index(0),
      _runningSum(0)
{
}

void Hx711::init()
{
    /* Enable GPIO clocks */
    if (_dtPort == GPIOA || _sckPort == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
    if (_dtPort == GPIOB || _sckPort == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    if (_dtPort == GPIOC || _sckPort == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* SCK -> Output */
    GPIO_InitStruct.Pin = _sckPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_sckPort, &GPIO_InitStruct);

    /* DT -> Input */
    GPIO_InitStruct.Pin = _dtPin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(_dtPort, &GPIO_InitStruct);

    HAL_GPIO_WritePin(_sckPort, _sckPin, GPIO_PIN_RESET);

    /* Initialize moving average buffer */
    _runningSum = 0;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        _buffer[i] = _offset;
        _runningSum += _offset;
    }
}

bool Hx711::readRaw(long &value)
{
    long count = 0;
    uint32_t timeout = HAL_GetTick();

    /* Wait until DT goes LOW (data ready) */
    while (HAL_GPIO_ReadPin(_dtPort, _dtPin))
    {
        if ((HAL_GetTick() - timeout) > 100)   // 100ms timeout
            return false;
    }

    for (int i = 0; i < 24; i++)
    {
        HAL_GPIO_WritePin(_sckPort, _sckPin, GPIO_PIN_SET);
        short_delay();

        count <<= 1;

        HAL_GPIO_WritePin(_sckPort, _sckPin, GPIO_PIN_RESET);
        short_delay();

        if (HAL_GPIO_ReadPin(_dtPort, _dtPin))
            count++;
    }

    /* 25th pulse -> Gain 128 */
    HAL_GPIO_WritePin(_sckPort, _sckPin, GPIO_PIN_SET);
    short_delay();
    HAL_GPIO_WritePin(_sckPort, _sckPin, GPIO_PIN_RESET);
    short_delay();

    /* Sign extend 24-bit value */
    if (count & 0x800000)
        count |= ~0xFFFFFF;

    value = count;
    return true;
}

void Hx711::movingAverage(long newValue)
{
    _runningSum -= _buffer[_index];
    _buffer[_index] = newValue;
    _runningSum += newValue;

    _index++;
    if (_index >= BUFFER_SIZE)
        _index = 0;
}

long Hx711::getWeight_x100()
{
    long raw;

    if (!readRaw(raw))
        return 0;   // timeout protection

    movingAverage(raw);

    long average = _runningSum / BUFFER_SIZE;

    long weight_x100 = ((average - _offset) * 100L) / _scale;

    /* Dead zone ±2g */
    if (weight_x100 < 200 && weight_x100 > -200)
        weight_x100 = 0;

    return weight_x100;
}