#pragma once

#include "ActionID.hpp"


//#define FRAME_TIME_INITIAL_THRESHOLD 0.2f //the robot's cycle time will often start at a value less than 1 millisecond per frame
										  //therefore, a minimum frame time threshold must be set as
										  //calculating deltaT is only accurate for integer values

#define MAX_PARAMS 5
#define MAX_ACTIONS 5


struct Action{
	int ID = ACTION_EMPTY;
	float params[MAX_PARAMS]{};

	Action();
	Action(float a, float b, float c, float d, float e, int ID);

	Action(const Action& arg);
};

class Queue{
private:
	int iter = 0;
	
	unsigned long long int startTime = 0;
	
	int deltaT = 1;
	int timeAccumulator = 0;

	Action actionArr[MAX_ACTIONS]{};

	Action* curr = nullptr;
	Action* next = nullptr;

public:
	Queue();
	Queue(Action init[], int size);
	~Queue();

	bool pushToNextEmpty(Action action);
	void setRelative(Action action, int offset);
	void setAbsolute(Action action, int pos);

	void beginNext();
	
	void startFrame();
	int endFrame();	// returns the value of deltaT so the global program can access it

	void executeAction();
};

