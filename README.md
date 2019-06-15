# Sudoku-validator
Sudoku validator and solver using multi-threads, mutex and semaphores (C language)

27 threads are used in validator to check if the solution is valid or not. Validation is carried out by three main checks
- row check
- column check
- block check (3x3)

Mutex are used so that any kind of thread overlapping could be stopped in validations case. 

The header file contains the definition of the structures and the 2D array of 9x9 that are to be checked against the solution validator. 
Some arrays represent a valid solution for the Sudoku board and others Invalid solution. The purpose for the header file is to make
the code less complex and more understandable.

In main function the array of threads has been declared and the threads created using pthread_create() function. 2D loop is being run 
for the Sudoku grid and checks applied for rows, columns and grid validity


