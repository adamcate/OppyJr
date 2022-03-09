#pragma once

#include "ActionID.hpp"


#define MAX_PARAMS 5
#define MAX_ACTIONS 5
typedef unsigned char uint8_t;

struct Action{
	int ID = ACTION_EMPTY;
	float params[MAX_PARAMS]{};

	bool completionInterrupt = false;	// TODO: interrupts call other functions based on ID after action completion
	int interruptID = -1;

	Action();
	Action(float a, float b, float c, float d, float e, int ID);

	Action(const Action& arg);
};

class Queue{
private:
	int wheelSpeed[2]{};

	int iter = 0;
	int currFlag = -1;
	
	unsigned long long startTime = 0;
	
	long deltaT = 1;
	double timeAccumulator = 0;

	Action actionArr[MAX_ACTIONS]{};

	Action* curr = nullptr;
	Action* next = nullptr;

public:
	Queue();
	Queue(Action init[], int size);
	~Queue();


	int getWheelSpeed(bool side);				//side false: left, side true: right
	bool pushToNextEmpty(Action action);		//push selected action at next available slot
	void setRelative(Action action, int offset);//set action at relative index to current
	void setAbsolute(Action action, int pos);	//set action at absolute index

	int getFlag();

	void beginNext();							//end current action and start next
	
	void startFrame();							//get the time at beginning of frame
	long endFrame();								//determine the time elapsed per frame

	void executeAction();						//execute/update the current action(called per frame)
};

