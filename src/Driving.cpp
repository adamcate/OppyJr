#include "Arduino.h"
#include "Driving.hpp"

void drive(int left, int right) // sign indicates direction
{
    if(left > 255) //prevent integer overflow in digitalWrite
        left = 255;
    else if(left < -255)
        left = -255;

    if(right > 255)
        right = 255;
    else if(right < -255)
        right = -255;

    digitalWrite(DIR_A,left >= 0 ? HIGH : LOW);
    digitalWrite(DIR_B,right >= 0 ? HIGH : LOW);

    analogWrite(PWM_A, abs(left));
    analogWrite(PWM_B, abs(right));
}