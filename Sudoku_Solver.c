#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include"SaveOS.h"
#include<stdbool.h>
#include <semaphore.h>


sem_t semP;				//Semophores dec
int N=9;
#define total_threads 27				//Change this to 11 and uncomment the code for 11 threads.

void *isvalid[total_threads]={0};			//if all 1(s) than valid solution
int my_arr[total_threads]={0};
int total_invalid=0;
pthread_mutex_t lock;			//Mutex dec for critical sections and exclusive access.


					//--A mutex is a lockable object that is designed to signal when 
					//critical sections of code need exclusive access, preventing 
					//other threads with the same protection from executing concurrently 
					//and access the same memory locations.--


					// ------SUDOKU CONDITION CHECK(s)------
					// 1-> Number non-repetative (in row)
					// 2-> Number non-repetative (in column)
					// 3-> Number non-repetative (in 3x3 block)
					// -------------IS VALID--------------

int main(void){
	
	sem_init(&semP, 0, 0);
	pthread_t mythread[total_threads];
	int valid_itr=0;
	int mytemp_var=0, mytemp_var2=1;
	pthread_mutex_init(&lock,NULL);

	printf("\n\n-------ORIGINAL SUDOKU GRID-------\n\n");
	printGrid();
	printf("\n\n");
    if (pthread_mutex_init(&lock, NULL) != 0){		//if not null/error during initialization
        printf("\n Mutex init failed\n");
        return 1;
	}

	for (int i=0;i<9;i++){
		
		for (int j=0;j<9;j++){
			
			if (i==0)							
			{
				parameters *my_ptr1 = (parameters *) malloc(sizeof(parameters));
				my_ptr1->column=j;
				my_ptr1->row=i;				
				my_ptr1->board = sudoku;   	
				pthread_create(&mythread[valid_itr], NULL, columncheck, (void *) my_ptr1);
				valid_itr++;
			}
			if (j==0)
			{
				parameters *my_ptr1 = (parameters *) malloc(sizeof(parameters));
				my_ptr1->row=i;
				my_ptr1->column=j;
				my_ptr1->board = sudoku; 
				pthread_create(&mythread[valid_itr], NULL, rowcheck, (void *) my_ptr1);
				valid_itr++;
			}
			if (i%3==0 && j%3==0)
			{	
				parameters *my_ptr3=(parameters *)malloc(sizeof(parameters));
				my_ptr3->row=i;
				my_ptr3->column=j;
				my_ptr3->board = sudoku;				
				pthread_create(&mythread[valid_itr],NULL,blockcheck,(void *) my_ptr3);
				valid_itr++;
			}
		}
	}


	int temp=0;
	while(temp<total_threads){					//waiting for all threads to complete cal
		pthread_join(mythread[temp],&isvalid[temp]);
		temp++;
	}

	pthread_mutex_destroy(&lock);					//mutex destroying.

	int count=0;
	for (int i=0;i<total_threads;i++){				
		if(isvalid[i]!=NULL){
			count++;
			my_arr[i]=1;
		}
		else{
			my_arr[i]=0;
		}
	}
	printf("\nTHREAD CHECK ARRAY\n");
	for(int i=0;i<total_threads;i++)				//bool array storing the results of all 11 threads
	{
		printf("%d",my_arr[i]);
	}
	if (count==total_threads)					//validity check
	{
		printf("\n------VALID SOLUTION------\n");
	}
	else{
		printf("\n TOTAL INVALID ENTRIES :%d",total_invalid);
		printf("\n------INVALID SOLUTION------\n");
	}
		
	printf("\n\n-------UPDATED SUDOKU GRID-------\n\n");
	printGrid();
	printf("\n\n");
	printf("---------SOLVED SUDOKU---------");
	printf("\n\n");	
	return 0;
}


void *rowcheck(void *pointer){
	sem_post(&semP);	
	pthread_mutex_lock(&lock);				//locking in the begining so that no content/context switching
	parameters *Para=(parameters *) pointer;		//row,column for checking(Num row repetation).
	int row,column;
	column=Para->column;
	row= Para->row;
	pid_t tid;
	int i=row,num;
	int j=column;	
	//bool numbervalid[10]={false};
	while(i<9)						//checks no(s) from 1-9 are non repetative in a row
	{	
		bool numbervalid[10]={false};
		
		while (j<9){
			num=Para->board[i][j];
			if (num<0 || num>9 || numbervalid[num]!= false){		//if not valid
				total_invalid++;
				printf("\n(ROW INVALID) DETAILS\n");
				printf("THREAD ID : %ld",pthread_self());
				printf("\nRow :%d",i+1);
				printf("\nColumn :%d",j+1);	
				printf("\n");			
				printf("Value :%d",num);
				printf("\n");	
				//sem_post(&semP);
				Solver(i,j); 
				//printGrid(); 
				
				//pthread_exit(NULL);
				pthread_mutex_unlock(&lock);	//unlocking if invalid			 
				return (void *) 0;
			}
			else{
				 numbervalid[num]= true;			
			}
			j++;
		}
		//printf("THREAD ID : %ld",pthread_self());
		i++;
	}
	//isvalid[9]=true;					//if numbers valid,check array true
	//pthread_exit(NULL);
	sem_wait(&semP);
	pthread_mutex_unlock(&lock);				//unlocking if valid;
	 return (void *) 1;
}

void *columncheck(void *pointer){
	sem_post(&semP);	
	pthread_mutex_lock(&lock);	
	parameters *Para=(parameters *) pointer;		//row,column for checking(Num column repetation).
	int row,column;
	int i,j,num;	
	//pid_t tid;
	//bool numbervalid[10]={false};	
	column=Para->column;
	row= Para->row;
	i=column;
	j=row;
	while(i<9)						//checks no(s) from 1-9 are non repetative in a column
	{
		bool numbervalid[10]={false};
		
		while(j<9){
			num=Para->board[j][i];
			if (num<0 || num>9 || numbervalid[num]!= false){		//if number already exists or number out of bounds
				total_invalid++;
				printf("\n(COLUMN INVALID) DETAILS\n");
				printf("THREAD ID : %ld",pthread_self());
				printf("\nRow :%d",j+1);
				printf("\n");
				printf("Column :%d",i+1);	
				printf("\n");			
				printf("Value :%d",num);
				printf("\n");					
				//pthread_exit(NULL);
			//	sem_post(&semP);
				Solver(i,j); 
			//	printGrid(); 

				pthread_mutex_unlock(&lock);
				return (void *) 0;		
			}
			else{
				numbervalid[num]= true;			
			}
			j++;
		}
		//printf("THREAD ID : %ld",pthread_self());
		i++;
	}
	sem_wait(&semP);
	pthread_mutex_unlock(&lock);
	return (void *) 1;	
}

void *blockcheck(void *pointer){				//3x3 block check(for non repetation of numbers in block)
	sem_post(&semP);	
	pthread_mutex_lock(&lock);
	parameters *Para=(parameters *) pointer;		//row,column for checking(Num column repetation).
	//pid_t tid;	
	int row,column;
	column=Para->column;
	row= Para->row;
	bool numbervalid[10]={false};
	int i=row;
	int j=column;
	for(;i<row+3;i++){
		for(;j<column+3;j++){
			int num=Para->board[i][j];
			if (numbervalid[num]!= false){
				
				
				printf("\nBLOCK DETAILS\n");
				printf("THREAD ID : %ld",pthread_self() );
				printf("\nRow :%d",row+1);
				printf("\n");
				printf("Column :%d",column+1);	
				printf("\n");			
				printf("Value :%d",num);
				printf("\n");
				
				Solver(i,j);
				// printGrid(); 
			
				pthread_mutex_unlock(&lock);
				return (void *) 0;	
			}
			else{
				numbervalid[num]=true;			
			}
		}
	//printf("THREAD ID : %ld",pthread_self());
	}
	sem_wait(&semP);
	pthread_mutex_unlock(&lock);
	return (void *) 1;
}
						
						//------------------PHASE-2--------------------------
						//------------------FOR SUDOKU SOLVING---------------

int Solver(int row,int col)			// Check that each number picked from Sudoku array doesn't  							// repeat in row, column and block respectively.
{	int t1=row;
	int t2=col;
	int temp=sudoku[row][col];
	for (int i=1;i<=9;i++){		
	if (isSafe(row, col, i)){  		//if number is clear then it is replaced/put in Sudoku Grid
            sudoku[row][col] = i; 
		printf("%d",temp);
		printf(" swapped with %d",i);
		printf("\nNEW ROW : %d",row+1);
		printf("\nNEW COLUMN : %d",col+1);
		return 1; 
  		
		row=t1;
		col=t2; 	
            sudoku[row][col] = temp; 		//else old number is put back in Sudoku Grid
        } 
    } 
	
    return 0;
}

int isSafe(int row, int col, int num) 		//checks that the number selected doesn't repeat in (Row, Col and Block respectively)
{ 
    return (!InRow(row, num) && !InCol(col, num)) && !InBox(row - row%3 , col - col%3, num); 
} 


int InRow(int row, int num) 			//Check that number doesn't repeat in the complete Sudoku row.
{ 
    for (int col = 0; col < N; col++){ 
        if (sudoku[row][col] == num){ 
            return  1;
	}
    }
    return 0;
} 

int InCol(int col, int num) 			//Check that number doesn't repeat in the complete Sudoku column.
{ 
    for (int row = 0; row < N; row++){ 
        if (sudoku[row][col] == num){
            return  1;
	}
    } 
    return 0; 
} 


int InBox(int StartRow, int StartCol, int num) //Check that number doesn't repeat in the Sudoku box/block.
{ 
    for (int row = 0; row < 3; row++){ 
        for (int col = 0; col < 3; col++){ 
            if (sudoku[row+StartRow][col+StartCol] == num){ 
                return 1;}
	}
    } 
    return 0; 
} 

void printGrid() 				//Utitlity fuction for Sudoku printing
{ 
    for (int row=0;row<N;row++) 
    { 
       for (int col=0;col<N;col++){ 
             printf("%2d",sudoku[row][col]); 
        }
	printf("\n"); 
    } 
} 


