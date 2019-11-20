#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <time.h>
#include <chrono> 
using namespace std::chrono; 

using namespace std; 

#define NUM_THREADS 8
#define matrix_size 200

// Matrix Allocation
int mat_A[matrix_size][matrix_size]; 
int mat_B[matrix_size][matrix_size]; 
int mat_result[matrix_size][matrix_size];
int mat_result_serial[matrix_size][matrix_size]; 

// Worker threads
void * multiply( void *arg )
{
  int tid, portion_size, row_start, row_end;
  
  tid = *(int *)(arg); // get thread ID 
  portion_size = matrix_size / NUM_THREADS;
  row_start = tid * portion_size;
  row_end = (tid+1) * portion_size;

  for (int i = row_start; i < row_end; ++i) { 
    for (int j = 0; j < matrix_size; ++j) {
      for (int k = 0; k < matrix_size; ++k) { 
	       mat_result[ i ][ j ] += mat_A[ i ][ k ] * mat_B[ k ][ j ];
      }
    }
  }
  return 0;
}

int main(){
	// Matrix initialization
	for (int i = 0; i < matrix_size; i++) { 
        for (int j = 0; j < matrix_size; j++) { 
            mat_A[i][j] = j;
            mat_B[i][j] = j; 
        } 
    } 

	// creating threads and assigning tasks to them 
	auto start = high_resolution_clock::now(); // starting time 
	pthread_t threads[NUM_THREADS]; 
	for (int i=0; i< NUM_THREADS; i++){
		int *tid;
		tid = (int *) malloc( sizeof(int) );
		*tid= i;
		pthread_create(&threads[i], NULL, multiply, (void*)tid); 
	}

	// Joining threads
	for (int i=0; i< NUM_THREADS; i++){
		pthread_join( threads[i], NULL); 
	}
	auto stop = high_resolution_clock::now();  //stop time
	auto duration = duration_cast<microseconds>(stop - start); 
	cout << "Time taken by " <<NUM_THREADS<< " threads in microseconds= "<< duration.count() << endl;

	// Print result
	// I used this part only on 4x4 matrices to make sure my results are correct then commented it

   // Print result 
	//for (int i = 0; i < matrix_size; i++) { 
      //  for (int j = 0; j < matrix_size; j++)  
        //    cout << "result ["<<i<<","<<j<<"]= "<<mat_result[i][j] << endl;         
	//} 


	// serial program  
	auto start_serial = high_resolution_clock::now(); // starting time 
	for (int i = 0;i < matrix_size; i++){// hold row index of 'matrix1'
    for (int j = 0; j < matrix_size; j++){// hold column index of 'matrix2'
      for (int k = 0; k < matrix_size;k++){ 
	       mat_result_serial[ i ][ j ] += mat_A[ i ][ k ] * mat_B[ k ][ j ];
      }
    }
  }
	auto stop_serial = high_resolution_clock::now();  //stop time
	auto duration_serial = duration_cast<microseconds>(stop_serial - start_serial); 
	cout << "Time taken by serial program= " <<duration_serial.count() << endl;
	
	getchar();

return 0;
}