#pragma once

extern "C" {
#include "main.h"
}

/*
 * Basic system initialization layer
 * Abstracts CubeMX generated init code
 */

class Bios
{
public:
    static void init();
};
