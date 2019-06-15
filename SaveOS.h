
// gcc phase1.c -o phase1 -lpthread

#ifndef SAVEOS_H
#define SAVEOS_H
#include <stdio.h>
#include <stdlib.h>

/*
int sudoku[9][9] = {						//Main Sudoku grid
	{6, 2, 4, 5, 3, 9, 1, 8, 7},
	{5, 1, 9, 7, 2, 8, 6, 3, 4},
	{8, 3, 7, 6, 1, 4, 2, 9, 5},
	{1, 4, 3, 8, 6, 5, 7, 2, 9},
	{9, 5, 8, 2, 4, 7, 3, 6, 1},
	{7, 6, 2, 3, 9, 1, 4, 5, 8},
	{3, 7, 1, 9, 5, 6, 8, 4, 2},
	{4, 9, 6, 1, 8, 2, 5, 7, 3},
	{2, 8, 5, 4, 7, 3, 9, 1, 6}
};*/



int sudoku[9][9] = {						//Invalid Sudoku grid(test-run)
	{6, 6, 4, 5, 3, 9, 1, 8, 7},
	{5, 1, 9, 7, 2, 8, 6, 3, 4},
	{8, 3, 7, 6, 1, 4, 2, 9, 5},
	{1, 4, 3, 3, 6, 5, 7, 2, 9},
	{9, 5, 8, 2, 4, 7, 3, 6, 1},
	{7, 6, 2, 3, 9, 1, 4, 5, 8},
	{3, 7, 1, 9, 5, 6, 8, 4, 2},
	{4, 9, 6, 1, 8, 2, 5, 7, 3},
	{2, 8, 5, 4, 7, 3, 9, 1, 6}
};


			//------------CONDITION CHECKS FOR SUDOKU------------

void *rowcheck(void *pointer);
void *columncheck(void *pointer);
void *blockcheck(void *pointer);

			//------------------FOR SUDOKU SOLVING---------------

int Solver(int row,int col);
int isSafe(int row, int col, int num); 
int InRow(int row, int num); 
int InCol(int col, int num); 
int InBox(int StartRow, int StartCol, int num);
void printGrid();



			//---------------------------------------------------
typedef struct							//parameters (for passing via pthreads)
{
int row;
int column;
int (* board)[9];						//(Alternative way) for 11 threads.
} parameters;

#endif







