#ifndef HX711_H
#define HX711_H

#include "main.h"

class HX711 {
public:
    // Constants from your calibration
    static constexpr long SCALE = 2376L;
    static constexpr long OFFSET = 1852778L;
    static constexpr int BUFFER_SIZE = 10;

    HX711(GPIO_TypeDef* dout_port, uint16_t dout_pin,
          GPIO_TypeDef* sck_port, uint16_t sck_pin);

    void Init();
    float GetWeight();
    float GetCalibratedWeight(long adc);
    long ReadRaw(void);

////    bool ReadRaw(long &value);
////    long Get_Raw();
////    long Get_Raw_Avj_Value(); // Returns weight * 100 (e.g., 1250 for 12.50g)
////    void TestSCK(uint16_t delayMs);
////    bool GetWeight(long &value);
//    float GetCalibratedWeight(long adc);

private:
    GPIO_TypeDef* _dout_port;
    uint16_t _dout_pin;
    GPIO_TypeDef* _sck_port;
    uint16_t _sck_pin;

    long _buffer[BUFFER_SIZE];
    int _index;
    long _runningSum;
};

#endif
