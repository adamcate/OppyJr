#include <Arduino.h>

#include "types.h"
#include "queue\queue.hpp"
#include "vec.h"
#include "bitmatrix.hpp"
#include "Raycast.hpp"
//#include "ArduinoNineAxisMotion.h"

bool getSensorOutputMean(u8 pin,u32 time,u16 threshold,Queue* queue);
void avoidObstacle();


Vec2 direction{};
Vec2 position{};
Vec2 detectPoint{};

const s16 numCasts = 15;

Vec3 raycastOutputs[numCasts]{};

const f32 sensingDist = 0.3f;     // measured in meters, ex 0.3f = 30cm

bitMatrix obstacleMap(10,10);

Queue queue{};
Queue avoidQueue{};

const Task _init_tasks[MAX_TASKS] = {DRIVE(255,255,1000),EMPTY(),EMPTY(),EMPTY(),EMPTY(),EMPTY(),EMPTY(),EMPTY(),EMPTY()};

const s16 sentinelNum = 3;
f32 sentinels[sentinelNum]{};

u16 IRsensorIterator = 0;


u32 timer = 0ul;
u32 accumulator = 0ul;


f32 angle = 0.f;


void setup() {
  for(u16 i = 0; i < MAX_TASKS; ++i) queue.pushToNextEmpty(_init_tasks[i]);
  queue.sentinel = sentinels;
  queue.sentinelSet = true;
  queue.sNumbers = sentinelNum;
}


void loop() {
  queue.startFrame();

  queue.executeAction();

  if(getSensorOutputMean(32,1000ul,2,&queue)){
    

    castRayFan(obstacleMap,raycastOutputs,position,3.f,angle,numCasts,100.f);
    // pick direction with surrounding points etc.

    
  }
  queue.endFrame();
}

bool getSensorOutputMean(u8 pin,u32 time,u16 threshold,Queue* queue){
  timer += queue->deltaT;
  accumulator += digitalRead(pin);
  if(!(timer >= time)) return false;

  return (accumulator / (timer / threshold)) != 0ul;
}

void avoidObstacle()
{
  detectPoint = position + direction * sensingDist; // set detect point based on the set calibration value
  setBitMatrixElement(obstacleMap,detectPoint.i,detectPoint.j,true);
  avoidQueue.setRelative(REVERSE_T(queue.wheelSpeed[0],queue.wheelSpeed[1],1000),1);

  while (magnitude(position - detectPoint) < 2.f * sqrtf(2.f) || getBitMatrixElement(obstacleMap, (s16)position.i, (s16)position.j)){
    avoidQueue.startFrame();

    integrateSensorReadings();

    sentinel[0] = getHeading();
    
    drive(-255,-255);

    avoidQueue.endFrame();
  }
}