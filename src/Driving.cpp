#include "Arduino.h"
#include "Driving.hpp"

void drive(int left, int right) // sign indicates direction
{
    digitalWrite(DIR_A,left >= 0 ? HIGH : LOW);
    digitalWrite(DIR_B,right >= 0 ? HIGH : LOW);

    analogWrite(PWM_A, abs(left));
    analogWrite(PWM_B, abs(right));
}