#include "Lerp.hpp"

float lerpf(float init, float fin, float t){
	return(init + (fin - init) * t);
}