#pragma once

//#define MEGA_PATCHED_OUT

#define MEGA

#ifdef MEGA
#define BRAKEVCC 0

#define PWM_1 45
#define PWM_2 46

#define CW_1  27
#define CCW_1 28

#define CW_2 25
#define CCW_2 26

#define BRAKEGND 3

#define EN_1 A0
#define EN_2 A1

#define CURR_SENS_1 A2
#define CURR_SENS_2 A3

#endif


#ifdef MEGA_PATCHED_OUT

#define BRAKEVCC 0

#define PWM_1 45
#define PWM_2 46

#define CW_1  47
#define CCW_1 48

#define CW_2 42
#define CCW_2 43

#define BRAKEGND 13

#define EN_1 A8
#define EN_2 A9

#define CURR_SENS_1 A10
#define CURR_SENS_2 A11


#endif

#define INIT_MOTOR_PINS(){/*macro shortcut for setting all nessesary pinmodes*/\
pinMode(PWM_1,OUTPUT);      \
pinMode(PWM_2,OUTPUT);      \
pinMode(CW_1,OUTPUT);       \
pinMode(CCW_1,OUTPUT);      \
pinMode(CW_2,OUTPUT);       \
pinMode(CCW_2,OUTPUT);      \
pinMode(CURR_SENS_1,OUTPUT);\
pinMode(CURR_SENS_2,OUTPUT);\
}                           
