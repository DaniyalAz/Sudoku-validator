# Sudoku-validator
Sudoku validator and solver using multi-threads, mutex and semaphores (C language)


Command to compile and run on linux(ubuntu): gcc Sudoku_Validator.c -o Sudoku_Validator -lpthread
---
Returns output on terminal.
---
I have made this project on ubuntu 18.04
---

Concepts used in this project are as following

-Processes
-pthreads (multi-thread)
-Mutex 
-Semaphores (binary)



#INTRODUCTION
--------------

#SODUOKU VALIDATOR
------------------
27 threads are used in validator to check if the solution is valid or not. Validation is carried out by three main checks
- row check
- column check
- block check (3x3)

Mutex are used so that any kind of thread overlapping could be stopped in validations case. 


#SUDOKU SOLVER
------------------
for soduku solver, same work has been done. the soduku validator is used to validate the position in the grid and after the valid poisition has been found the solver solves the grid by inserting a valid number in the row, column and block (3x3).
the main functions for solving the soduku are

--------------------------------------
These three functions check if the number is repeating in the row, column or box or not.
-InBox
-InCol
-InRow 

--------------------------------------
This function mentioned below varifies that the number selected is safe to be added into a position in the grid.
-isSafe

--------------------------------------
The funciton mentioned below picks a number from an invalid solution grid and checks if that number repeats in row, column or box so that it can either be replaced or kept in which position in the grid.

-Solver

---------------------------------------
Utility functions used

-printGrid


Semaphores are used in Sudoku solver part so that the thread overlapping and other problems before joining the threads can be avoided.

-------------------------

The header file contains the definition of the structures and the 2D array of 9x9 that are to be checked against the solution validator. 
Some arrays represent a valid solution for the Sudoku board and others Invalid solution. The purpose for the header file is to make
the code less complex and more understandable.

In main function the array of threads has been declared and the threads created using pthread_create() function. 2D loop is being run 
for the Sudoku grid and checks applied for rows, columns and grid validity

----------------------

