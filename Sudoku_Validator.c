#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include"SaveOS.h"
#include<stdbool.h>
//#include<mutex.h>

#define total_threads 27				//Change this to 11 and uncomment the code for 11 threads.

void *isvalid[total_threads]={0};			//if all 1(s) than valid solution
int my_arr[total_threads]={0};
//int row,column; 
//int idx1=NULL;
//int idx2=-1
//int idx3=0;
//bool numbervalid[9]={false};
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
	
	pthread_t mythread[total_threads];
	int valid_itr=0;
	pthread_mutex_init(&lock,NULL);
	//parameters *my_ptr1 = (parameters *) malloc(sizeof(parameters));
    	//my_ptr1->row = 0;
    	//my_ptr1->column = 0;
    	//my_ptr1->board = sudoku;
	//bool valid

	/*for (int i=0;i<9;i++)									//COMMENTED CODE IS FOR 11 THREADS
	{											//ROWS + COLUMN TREADS CREATED IN THIS 
		my_ptr1->row=i;									//WORKS BOTH FOR 11 AND 27 THREADS
	pthread_create(&mythread[0], NULL, rowcheck, (void *) my_ptr1);
	}
	for (int i=0;i<9;i++){ 
	my_ptr1->column=i;   	
	pthread_create(&mythread[1], NULL, columncheck, (void *) my_ptr1);
	}*/
//	pthread_create(&mythread[0],NULL,blockcheck,(void *) my_ptr3);
//	pthread_create(&mythread[1],NULL,blockcheck,(void *) my_ptr3);



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
	return 0;
}







void *rowcheck(void *pointer){
	pthread_mutex_lock(&lock);				//locking in the begining so that no content/context switching
	parameters *Para=(parameters *) pointer;		//row,column for checking(Num row repetation).
	int row,column;
	column=Para->column;
	row= Para->row;
	
	//if (row>8 || column!=0)				//Out of bound Rows, Column
	//{
	//	pthread_exit(NULL);
	//}
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
												
				//pthread_exit(NULL);
				pthread_mutex_unlock(&lock);	//unlocking if invalid			 
				return (void *) 0;
			}
			else{
				 numbervalid[num]= true;			
			}
			j++;
		}
		i++;
	}
	//isvalid[9]=true;					//if numbers valid,check array true
	//pthread_exit(NULL);
	pthread_mutex_unlock(&lock);				//unlocking if valid;
	 return (void *) 1;
}

void *columncheck(void *pointer){
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
				pthread_mutex_unlock(&lock);
				return (void *) 0;		
			}
			else{
				numbervalid[num]= true;			
			}
			j++;
		}
		i++;
	}
	pthread_mutex_unlock(&lock);
	return (void *) 1;	
}

void *blockcheck(void *pointer){				//3x3 block check(for non repetation of numbers in block)
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
				pthread_mutex_unlock(&lock);
				return (void *) 0;	
			}
			else{
				numbervalid[num]=true;			
			}
		}
	}
	pthread_mutex_unlock(&lock);
	return (void *) 1;
}

