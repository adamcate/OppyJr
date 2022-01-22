#pragma once

#include <Arduino.h>
#include "Pins.h"


// defined once and used everywhere, these are the pin numbers


void drive(int spdLeft, int spdRight);              // arguments can be negative; sign indicates motor direction