#include "BitMatrix.hpp"
#include <Arduino.h>
#include "Driving.hpp"

bitMatrix::bitMatrix()
{

}

bitMatrix::bitMatrix(int width, int height)
{
	initializeBitMatrix(*this, width, height);
}

bitMatrix::~bitMatrix()	// free matrix array memory when no longer used
{
	delete[] array;
	array = nullptr;
}

bool initializeBitMatrix(bitMatrix& matrix, int width, int height, unsigned char contents[])
{
	matrix.width = width;
	matrix.height = height;

	if (matrix.array != nullptr)					// if the memory in array is not free, delete the contents and free it
	{												//so it can be modified
		delete[] matrix.array;
		matrix.array = nullptr;
	}

	matrix.arrsize = ceil(float(width * height)/ 8);	// set the number of elements to the amount nessesary
	matrix.array = new unsigned char[matrix.arrsize];		//to hold the data

	for (int i = 0; i < matrix.arrsize; ++i)		// set the array equal to the contents
	{
		matrix.array[i] = contents[i];
	}
	return true;
}

void initializeBitMatrix(bitMatrix& matrix, int width, int height)
{
	matrix.width = width;
	matrix.height = height;

	if (matrix.array != nullptr)
	{											
		delete[] matrix.array;
		matrix.array = nullptr;
	}

	matrix.arrsize = ceil(float(width * height) / 8);
	matrix.array = new unsigned char[matrix.arrsize];

	for (int i = 0; i < matrix.arrsize; ++i)		// set the array equal to the contents
	{
		matrix.array[i] = (unsigned char)0x00;			// initialize as all zeros
	}
}


bool getBitMatrixElement(bitMatrix& matrix, int x, int y)
{
	if (x > matrix.width || y > matrix.height || x < 0 || y < 0)		// return false (0) if out of bounds
		return false;

	int fullIndex = y * matrix.width + x;			// fullIndex is the nth bit of the data
	int arrIndex = fullIndex / 8;					// integer division (truncation) to get the nth index of matrix.array
	int bitIndex = fullIndex % 8;					// the sub index bit of the nth index of the char in matrix.array

	unsigned char bitSet = matrix.array[arrIndex];		// the byte that contains the desired value

	return ((bitSet >> bitIndex) & 0x01) == 0x01;	// shift bitSet by the sub index and return as boolean
}

void setBitMatrixElement(bitMatrix& matrix, int x, int y, bool value)
{
	if (x > matrix.width || y > matrix.height || y < 0 || x < 0)		// exit if out of bounds
		return;

	int fullIndex = y * matrix.width + x;			// fullIndex is the nth bit of the data
	int arrIndex = fullIndex / 8;					// integer division (truncation) to get the nth index of matrix.array
	int bitIndex = fullIndex % 8;					// the sub index bit of the nth index of the char in matrix.array

	unsigned char inputSet = ((unsigned char)0x01) << bitIndex;
	
	if (value) {
		matrix.array[arrIndex] = matrix.array[arrIndex] | inputSet;
		return;
	}
	inputSet = ~inputSet;
	matrix.array[arrIndex] = matrix.array[arrIndex] & inputSet;
}


void printBitMatrix(bitMatrix& matrix)
{
	for (int i = 0; i < matrix.height; ++i)
	{
		for (int j = 0; j < matrix.width; ++j)
		{
			Serial.print(getBitMatrixElement(matrix, j, i));
		}
		Serial.println();
	}
	Serial.println();
}
