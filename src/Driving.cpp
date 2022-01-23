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

    int lLed = abs(left) / 82;
    int rLed = abs(right) / 82;

    for(int i = LED_L_1; i <= LED_L_3; ++i){
        if(i - LED_L_1 + 1<= lLed){
            digitalWrite(i,HIGH);
            continue;
        }
        digitalWrite(i,LOW);
    }

    for(int i = LED_R_3; i <= LED_R_1; ++i){
        if(LED_R_1 - i + 1<= rLed){
            digitalWrite(i,HIGH);
            continue;
        }
        digitalWrite(i,LOW);
    }


    digitalWrite(DIR_A,left >= 0 ? HIGH : LOW);
    digitalWrite(DIR_B,right >= 0 ? HIGH : LOW);

    analogWrite(PWM_A, abs(left));
    analogWrite(PWM_B, abs(right));
}