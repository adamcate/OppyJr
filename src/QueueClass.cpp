#include "QueueClass.hpp"
#include "Driving.hpp"
#include "Lerp.hpp"
#include "ActionID.hpp"


Action::Action(){
	
}

Action::Action(float a, float b, float c, float d, float e, int ID){
	params[0] = a;
	params[1] = b;
	params[2] = c;
	params[3] = d;
	params[4] = e;

	this->ID = ID;
}

Action::Action(const Action& arg){
	this->ID = arg.ID;
	for(int i = 0; i < MAX_PARAMS; i++)
		this->params[i] = arg.params[i];

	completionInterrupt = arg.completionInterrupt;	// TODO: interrupts call other functions based on ID after action completion
	interruptID = arg.interruptID;
}

Queue::Queue(){
	curr = &actionArr[0];
	next = &actionArr[1];
}

Queue::Queue(Action init[], int size){
	for(int i = 0; i < size; ++i)
		this->setAbsolute(init[i],i);
}

Queue::~Queue(){ // take care of dangling pointers
	curr = nullptr;
	next = nullptr;
}

bool Queue::pushToNextEmpty(Action action){
	for(int i = 0; i < MAX_ACTIONS; ++i){
		if(actionArr[(iter+i)%MAX_ACTIONS].ID == ACTION_EMPTY){
			iter = 0;
			actionArr[(iter+i)%MAX_ACTIONS] = action;
			return true;
		}
	}
	return false;
}

void Queue::setRelative(Action action, int offset){
	actionArr[(iter+offset)%MAX_ACTIONS] = action;
	actionArr[(iter+offset)%MAX_ACTIONS].interruptID = action.interruptID;
}

void Queue::setAbsolute(Action action, int pos){
	if(pos < 0 || pos > MAX_ACTIONS - 1)
		return;
	actionArr[pos] = action;
}

void Queue::beginNext(){
	currFlag = curr->interruptID;
	curr = next;
	next = &actionArr[(iter+1)% MAX_PARAMS];

	timeAccumulator = ((double)deltaT) / 1000.;

	if(++iter >= MAX_ACTIONS)
		iter = 0;
}
	
void Queue::startFrame(){ // TODO change program logic to use micros() instead of millis() for better accuracy
	startTime = micros();
	currFlag = -1;
}

long Queue::endFrame(){
	deltaT = micros() - startTime + 1;
	return deltaT;
}	// returns the value of deltaT so the global program can access it

void Queue::executeAction(){	// really gross long switch statement
    float frac = 0.0;

	if(curr->ID == ACTION_EMPTY) 
	{
		beginNext();
		return;
	}
	timeAccumulator += (float)(deltaT / 1000);

	switch((*curr).ID)
	{

	case ACTION_EMPTY:
        beginNext();
        break;

    case ACTION_DRIVE:
		frac = timeAccumulator / curr->params[2];
		//Serial.println(frac);
		
		wheelSpeed[0] = curr->params[0];
		wheelSpeed[1] = curr->params[1];

        drive(wheelSpeed[0],wheelSpeed[1]);
        if(timeAccumulator >= curr->params[2]){
            beginNext();
            return;
		}
        break;

    case ACTION_DRIVE_T:
		frac = timeAccumulator / curr->params[4];

		wheelSpeed[0] = lerpf(curr->params[0],curr->params[1],frac);
		wheelSpeed[1] = lerpf(curr->params[2],curr->params[3],frac);

        drive(wheelSpeed[0],wheelSpeed[1]);
        if(timeAccumulator >= curr->params[4]){
            beginNext();
            return;
		}
        break;

    case ACTION_REVERSE_T:
        if(timeAccumulator >= curr->params[2]){
            beginNext();
            return;
		}
        frac = timeAccumulator / curr->params[2];
		wheelSpeed[0] = lerpf(curr->params[0],-1 * curr->params[0],frac);
		wheelSpeed[1] = lerpf(curr->params[1],-1 * curr->params[1],frac);

        drive(wheelSpeed[0],wheelSpeed[1]);

        break;

    case ACTION_WAIT_T:
        break;

    case ACTION_WAIT:
        break;
        
    default:
        break;
	}
}

int Queue::getFlag()
{
	return currFlag;
}

int Queue::getWheelSpeed(bool side)
{
	if(side) return wheelSpeed[1];
	return wheelSpeed[0];
}