#pragma once

/*
Code to reduce the memory footprint of boolean-like matricies by a factor of 8.
*/


struct bitMatrix
{
	int width = 0, height = 0;			//defaults to 0 until initialized
	unsigned char* array = nullptr;			//bit array contents (pointer to null until initialized)
	int arrsize = 8;					

	bitMatrix();						//default constructor
	bitMatrix(int width, int height);	//specify height in constructor
	~bitMatrix();						//free program memory when out of scope
};


bool initializeBitMatrix(bitMatrix& matrix, int width, int height, unsigned char contents[]);	//initialize the bitMatrix with a set of contents
void initializeBitMatrix(bitMatrix& matrix, int width, int height);						//initialize the bitMatrix as empty (0s)

bool getBitMatrixElement(bitMatrix& matrix, int x, int y);								//return the value at a given x and y index of the matrix
void setBitMatrixElement(bitMatrix& matrix, int x, int y, bool value);					//set the bit at x and y

void printBitMatrix(bitMatrix& matrix);													//print a graphical representation of the matrix