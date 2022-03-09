#pragma once

// lay out parameter specifications etc.

#define ACTION_EMPTY 0
#define ACTION_DRIVE 1  // left, right, time
#define ACTION_DRIVE_T 2 // left1, left2, right1, right2, time
#define ACTION_REVERSE_T 3 // left, right, reverseTime
#define ACTION_WAIT 4
#define ACTION_WAIT_T 5
#define ACTION_ROTATE 6
#define ACTION_HALT 7

// TODO create macros to make typing easier

#include "QueueClass.hpp"

#define EMPTY() Action()
#define DRIVE(left, right, time) Action(left,right,time,0.f,0.f,ACTION_DRIVE)
#define DRIVE_T(left1, left2, right1, right2, time) Action(left1, left2, right1, right2, time, ACTION_DRIVE_T)
#define REVERSE_T(left,right,time) Action(left,right,time,0.f,0.f,ACTION_REVERSE_T)
#define HALT(time) Action()  // TODO implement full stop code
#define ROTATE(angle) Action() // TODO implement rotation

#define FLAG_SETUP_COMPLETE 0x01
#define SEGMENT_COMPLETE 0x02