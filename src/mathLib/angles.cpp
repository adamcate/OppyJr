#include "mathLib\angles.hpp"
#include "types.h"

f32 angleDiff(f32 angleInitial, f32 angleFinal)
{
	f32 stepPositive = angleFinal - angleInitial;
	f32 stepNegative = angleFinal - angleInitial - 360.f;

	if (fabs(stepPositive) <= fabs(stepNegative)) return stepPositive;

	return stepNegative;
}

f32 getAngle(f32 x, f32 y) 
{
	f32 angle = atan2f(y,x) * 57.3248f;
	if (angle >= 0.f) return angle;
	return 360.f + angle;
}
