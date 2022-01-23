/*
COSGC robotics team OppyJr code
Adam Cate
*/

#include <Arduino.h>
#include "OppyJr.hpp"
#include "vec.h"


Queue queue{};

//Action _init_actions[5] = {DRIVE_T(0,0,255,255,5000),REVERSE_T(255,255,5000), DRIVE(-255,-255,5000), 
//DRIVE_T(-255,-255,255,255,5000),DRIVE_T(255,255,0,0,3000)};// array of actions used at program startup

Action _init_actions[5] = {DRIVE_T(0,255,0,255,12000),DRIVE_T(255,255,255,255,12000), DRIVE_T(255,-255,255,-255,6000), 
EMPTY(),DRIVE_T(-255,0,-255,0,12000)};// array of actions used at program startup

void setup()
{
	// pin numbers as defined in Driving.hpp
	pinMode(PWM_A, OUTPUT);
	pinMode(PWM_B, OUTPUT);
	pinMode(DIR_A, OUTPUT);
	pinMode(DIR_B, OUTPUT);

	for (int i = 0; i < 5; i++)					// push all actions from _init_actions to the queue
	{
		queue.pushToNextEmpty(_init_actions[i]);
	}

	Serial.begin(9600);
}

void loop()
{
	queue.startFrame();
	
	queue.executeAction();

	queue.endFrame();
}