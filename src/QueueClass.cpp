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

void Queue::setRelative(Action action, int offset){actionArr[(iter+offset)%MAX_ACTIONS] = action;}

void Queue::setAbsolute(Action action, int pos){
	if(pos < 0 || pos > MAX_ACTIONS - 1)
		return;
	actionArr[pos] = action;
}

void Queue::beginNext(){
	curr = next;
	next = &actionArr[(iter+1)% MAX_PARAMS];

	timeAccumulator = 0;

	if(++iter >= MAX_ACTIONS)
		iter = 0;
}
	
void Queue::startFrame(){ // TODO change program logic to use micros() instead of millis() for better accuracy
	startTime = millis();
}

int Queue::endFrame(){
	deltaT = millis() - startTime + 1;
	return deltaT;
}	// returns the value of deltaT so the global program can access it

void Queue::executeAction(){	// really gross long switch statement
    float frac = 0.0f;

	timeAccumulator += deltaT;

	switch((*curr).ID)
	{

	case ACTION_EMPTY:
        beginNext();
        break;

    case ACTION_DRIVE:
        if(timeAccumulator >= curr->params[2]){
            beginNext();
            return;
		}

        drive(curr->params[0],curr->params[1]);
        break;

    case ACTION_DRIVE_T:
        if(timeAccumulator >= curr->params[4]){
            beginNext();
            return;
		}
        frac = timeAccumulator / curr->params[4];
        drive(lerpf(curr->params[0],curr->params[1],frac),lerpf(curr->params[2],curr->params[3],frac));
        break;

    case ACTION_REVERSE_T:
        if(timeAccumulator >= curr->params[2]){
            beginNext();
            return;
		}
        frac = timeAccumulator / curr->params[2];
        drive(lerpf(curr->params[0],-1 * curr->params[0],frac),lerpf(curr->params[1],-1 * curr->params[1],frac));
        break;

    case ACTION_WAIT_T:
        break;

    case ACTION_WAIT:
        break;
        
    default:
        break;
	}
}