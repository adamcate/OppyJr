
#include "Raycast.hpp"
#include <Arduino.h>


Vec3 castRay(bitMatrix& grid, int gridScale, Vec2 posInitial, Vec2 posFinal)
{
	Vec2 direction = posFinal - posInitial;

	int gridIndexX = posInitial.i, gridIndexY = posInitial.j; //conversion to int truncates

	int stepX = 0, stepY = 0;
	// initialize the step direction for X and Y
	if (direction.i > 0) stepX = 1;
	else if (direction.i < 0) stepX = -1;
	else stepX = 0;

	if (direction.j > 0) stepY = 1;
	else if (direction.j < 0) stepY = -1;
	else stepY = 0;

	if (stepX == 0)
	{
		float tDeltaY = 1.f / direction.j * stepY;

		float tMaxY = ((float)(stepY * (gridIndexY - posInitial.j + 0.5) + 0.5 * gridScale)) / direction.j * stepY;
		while (true)
		{
			gridIndexY += stepY;
			tMaxY += tDeltaY;
			if (getBitMatrixElement(grid, gridIndexX, gridIndexY) || gridIndexY >= grid.height || gridIndexY < 0) break;
		}

		float fPos = posInitial.j + direction.j * (tMaxY - tDeltaY);
		return(Vec3(posInitial.i, fPos,abs(fPos - posInitial.j)));
	}

	//float slope = direction.j / direction.i;
	float arcLenFactor = sqrtf(direction.i * direction.i + direction.j * direction.j)/direction.i;

	float tMaxX = ((float)(stepX*(gridIndexX  - posInitial.i + 0.5) + 0.5 * gridScale)) / direction.i*stepX, tMaxY = ((float)(stepY*(gridIndexY - posInitial.j + 0.5) + 0.5 * gridScale)) / direction.j*stepY;

	float tDeltaX = 1.f / direction.i * stepX, tDeltaY = 1.f / direction.j * stepY;

	bool lastStep = false; // indicates the direction of the previous step (x or y increment) : false -> first if. true -> second

	while (true)
	{
		if (tMaxX < tMaxY) {
			lastStep = false;
			gridIndexX += stepX;

			//if (gridIndexX >= grid.width || gridIndexX < 0) return Vec3();

			tMaxX += tDeltaX;
		}
		else {
			lastStep = true;
			gridIndexY += stepY;

			//if (gridIndexY >= grid.height || gridIndexY < 0) return Vec3();
			
			tMaxY += tDeltaY;
		}

		if (getBitMatrixElement(grid, gridIndexX, gridIndexY) || gridIndexY >= grid.height || gridIndexY < 0 || gridIndexX >= grid.width || gridIndexX < 0) {
			if (!lastStep)
				return Vec3(posInitial.i + direction.i * (tMaxX - tDeltaX), posInitial.j + direction.j * (tMaxX - tDeltaX), arcLenFactor * (tMaxX - tDeltaX)*direction.i);
			return Vec3(posInitial.i + direction.i * (tMaxY - tDeltaY), posInitial.j + direction.j * (tMaxY - tDeltaY), arcLenFactor * (tMaxY - tDeltaY)*direction.i);
		}
		if (tMaxX - tMaxX>= 1.f || tMaxY - tMaxY >= 1.f)
		{
			return Vec3(posFinal.i,posFinal.j,magnitude(posFinal-posInitial));
		}
	}
}

void castRayFan(bitMatrix& matrix, Vec3 * outputs, Vec2 origin, float length, float initialAngle, int noRays, float angleSpread)
{
	float angle{};
	
	if(noRays % 2) return;

	for (int i = 0; i < noRays; ++i)
	{
		angle = initialAngle - angleSpread + (i)*angleSpread / (noRays / 2 + 1);
		outputs[i] = castRay(matrix, 1, origin, Vec2(length * cos(angle) + origin.i, length * sin(angle) + origin.j));
	}
}