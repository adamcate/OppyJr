#pragma once

#include "ActionID.hpp"


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
	
	long int startTime = 0;
	
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

