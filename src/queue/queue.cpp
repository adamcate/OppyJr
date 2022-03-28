#include "queue/queue.hpp"
#include "queue/driving.hpp"


Task::Task(){}

Task::Task(s16 args[], s16 numArgs,s8 ID){
    if (numArgs > MAX_PARAMS) return;
    for(s16 i = 0; i < numArgs; ++i)
        params[i] = args[i];
    this->ID = ID;
}
Task::Task(s16 a,s16 b,s16 c,s16 d,s16 e, s16 f, s8 ID){
    params[0] = a;
	params[1] = b;
	params[2] = c;
	params[3] = d;
	params[4] = e;
    params[5] = f;
    this->ID = ID;
}

Task::Task(const Task& task){
    ID = task.ID;
    for(s16 i = 0; i < MAX_PARAMS; ++i)
        params[i] = task.params[i];
    completionInterrupt = task.completionInterrupt;
    interruptID = task.interruptID;
}

// Queue constructors

Queue::Queue(){

}

Queue::Queue(Task init[], s16 sz){
	for(s16 i = 0; i < sz; ++i)
		setAbsolute(init[i],i);
}

Queue::~Queue(){ // take care of dangling pointers
	curr = nullptr;
	next = nullptr;
}
// add specified task to next empty index, return true if succeeded,
// return false if no empty slot was found
bool Queue::pushToNextEmpty(Task task){
	for(s16 i = 0; i < MAX_TASKS; ++i){
		if(tasks[(iter+i)%MAX_TASKS].ID == T_EMPTY){
			iter = 0;
			tasks[(iter+i)%MAX_TASKS] = task;
			return true;
		}
	}
	return false;
}
// set the task at an index relative to the current action:
// wraps around when exceeding size
void Queue::setRelative(Task task, s16 offset){
	tasks[(iter+offset)%MAX_TASKS] = task;
}
// set task at absolute index, unless out of bounds
void Queue::setAbsolute(Task task, u16 pos){
	if(pos < 0 || pos > MAX_TASKS - 1) return;
	tasks[pos] = task;
}
// step forward in Queue and reset timers
void Queue::beginNext(){
	currFlag = curr->interruptID;
	curr = next;
	next = &tasks[(iter+1)% MAX_TASKS];

	timeAccumulator = deltaT / 1000.f;

	if(++iter >= MAX_TASKS)
		iter = 0;
}
// get the start time & reset flag to -1
void Queue::startFrame(){
	startTime = micros();
	currFlag = -1;
}
// calculate frame time by time difference from startTime
void Queue::endFrame(){
	deltaT = micros() - startTime + 1ul;
}

void Queue::executeAction(){	// really gross long switch statement
	timeAccumulator += (f32)(deltaT / 1000.f);

    f32 frac = (timeAccumulator) / curr->params[5];

    switch(curr->ID){
        case T_EMPTY:
            beginNext();
            return;
        case T_DRIVE:
            wheelSpeed[LEFT] = curr->params[0];
		    wheelSpeed[RIGHT] = curr->params[1];
            drive(wheelSpeed[LEFT],wheelSpeed[RIGHT]);
            break;
        case T_DRIVE_T:
            wheelSpeed[LEFT] = lerpf(curr->params[0],curr->params[1],frac);
		    wheelSpeed[RIGHT] = lerpf(curr->params[2],curr->params[3],frac);
            drive(wheelSpeed[LEFT],wheelSpeed[RIGHT]);
            break;
        case T_REVERSE_T:
		    wheelSpeed[LEFT] = lerpf(curr->params[0],-1.f * curr->params[0],frac);
		    wheelSpeed[RIGHT] = lerpf(curr->params[1],-1.f * curr->params[1],frac);
            drive(wheelSpeed[LEFT],wheelSpeed[RIGHT]);
            break;
        case T_HALT:
            wheelSpeed[LEFT] = lerpf(curr->params[0],0.f,frac);
		    wheelSpeed[RIGHT] = lerpf(curr->params[1],0.f,frac);
            drive(wheelSpeed[LEFT],wheelSpeed[RIGHT]);
        case T_ROTATE:
            // TODO add watcher & ROTATE code
            /*
            if(!(absf(getAngleDiff(currAngle,targetAngle)) < 5.f))
            {
                drive() etc.
            }
            */
            beginNext();
            return;
        default:
            break;
    }
    if(timeAccumulator >= curr->params[5]){
        beginNext();
        return;
    }
}