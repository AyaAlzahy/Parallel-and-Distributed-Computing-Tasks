#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <time.h>
using namespace std; 

int main() {
	clock_t start_t, end_t;
	int n= 8*8;   // number of matrix rows and columns 
	float** matrix_a = new float*[n];       //first matrix 
    float** matrix_b = new float*[n];       // second matrix
    float** result = new float*[n];   //Result of the parallel program with size nxn. Result= matrix_a x matrix_b
    float** result_scalar = new float*[n];  //Result of the scalar program with size nxn
    
// dynamically allocate matrices
for (int i = 0; i < n; i++) {
	matrix_a[i] = new float[n];
	matrix_b[i] = new float[n];
	result[i] = new float[n];
	result_scalar[i] = new float[n];
}
 // initialize matrices
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix_a[i][j] = i;
			matrix_b[i][j] = j;
            result[i][j] = 0;
			result_scalar[i][j] = 0;

        }
    } 

	// Parallel Program 
start_t = clock();
printf("Parallel Program start_t = %ld\n", start_t);

for(int i = 0; i < n; i++){
        for(int k = 0; k < n; k++){
                //Equivalent to: result_scalar[i][j] += matrix_a[i][k] * matrix_b[k][j];
				__m256 matrix1 = _mm256_set1_ps(matrix_a[i][k]); // broadcast matrix a elements
                for(int j=0; j<n; j+= 8) {
                        __m256 output = _mm256_loadu_ps(&result[i][j]); // load 8 s precision from result
                        __m256 matrix2 = _mm256_loadu_ps(&matrix_b[k][j]); // load 8 s precision from matrix b
                        __m256 t1 = _mm256_mul_ps(matrix2, matrix1); // multiply
                        output = _mm256_add_ps(t1, output); // add to the result
                        _mm256_storeu_ps(&result[i][j],output); // store
                }
            }
		}

end_t = clock();
printf("end_t = %ld\n", end_t);
double total_t = (end_t - start_t);
total_t= total_t/ CLOCKS_PER_SEC;
printf("Total time: %f\n", total_t  );

	
	

	//////////////// SCALAR PROGRAM///////////////////
start_t = clock();
printf("Scalar Program start_t = %ld\n", start_t);

	for(int i = 0; i < n; i++){
        for(int k = 0; k < n; k++){
			for (int j=0; j<n; j++){ 
                result_scalar[i][j] += matrix_a[i][k] * matrix_b[k][j];
			}}}


end_t = clock();
printf("end_t = %ld\n", end_t);
total_t = (end_t - start_t);
total_t= total_t/ CLOCKS_PER_SEC;
printf("Total time: %f\n", total_t  );

/////////////////////////////// Print Program Outputs///////////////////////////

	cout <<" -------SCALAR-------\n"; 
	for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f\n ", result_scalar[i][j]);
        }
    }

	cout <<" -------Parallel Output-------\n"; 
	for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f\n ", result[i][j]);
        }
    }

	getchar();

	return 0; 
}