#include "Arduino.h"
#include "Driving.hpp"


void drive(int left, int right)
{
    if(left > 255) left = 255;
    else if (left < -255) left = -255;

    if(right > 255) right = 255;
    else if (right < -255) right = -255;

    digitalWrite(CW_1,left >= 0 ? HIGH : LOW);
    digitalWrite(CCW_1,left < 0 ? HIGH : LOW);
    analogWrite(PWM_1,abs(left));

    digitalWrite(CW_2,right >= 0 ? HIGH : LOW);
    digitalWrite(CCW_2,right < 0 ? HIGH : LOW);
    analogWrite(PWM_2,abs(right));
}