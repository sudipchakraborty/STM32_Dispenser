#ifndef DEBUG_HPP
#define DEBUG_HPP

extern "C" {
#include "main.h"
}

class Debug
{
public:
    static void init(uint32_t baudrate = 115200);
    static void print(const char* msg);
    static void println(const char* msg);
    static void printInt(int value);
    static void println(const char* label, long value);
};

#endif
