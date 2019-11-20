#include <malloc.h> 
#include <iostream>
#include <immintrin.h> 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std; 
// scalar function for computing sin approximation 
void sinx_scalar (int N, int terms, float *x, float *result){

clock_t start_t;
clock_t end_t;
start_t = clock();
printf("start_t = %ld\n", start_t);

	for (int i=0; i<N; i++){
		float value = x[i];
        float numer = x[i]*x[i]*x[i];
        int denom = 6; // 3!
        int sign = -1;

	    for (int j=1; j<=terms; j++) {
            value += sign * numer / denom;
            numer *= x[i] * x[i];
            denom *= (2*j+2) * (2*j+3);
            sign *= -1;
        }

        result[i] = value;
    }
end_t = clock();
printf("end_t = %ld\n", end_t);
double total_t = (end_t - start_t);
total_t= total_t/ CLOCKS_PER_SEC;
printf("Total time by the scalar program: %f\n", total_t  );
}

// Sin approximation using AVX Intrinsics 
void sinx_intrinsic(int N, int terms, float* x, float* result)
{
clock_t start_t;
clock_t end_t;
start_t = clock();
printf("start_t = %ld\n", start_t);

 for (int i=0; i<N; i+=8)
 {
	 // scalar version: float value = x[i];
     __m256 Xi = _mm256_load_ps(&x[i]);
     __m256 value = Xi;
	 //float numer = x[i]*x[i]*x[i];
     __m256 numer = _mm256_mul_ps(Xi, _mm256_mul_ps(Xi, Xi));
	 // int denom = 6; // 3!
     __m256 denom = _mm256_set1_ps(6);
	 // int sign *= -1;
     int sign = -1;

     for (int j=1; j<=terms; j++)
        {
          // value += sign * numer / denom
         value = _mm256_add_ps(value, _mm256_div_ps(_mm256_mul_ps(_mm256_set1_ps(sign), numer), denom));
		 //numer *= x[i] * x[i];
         numer = _mm256_mul_ps(numer, _mm256_mul_ps(Xi, Xi));
		 // denom *= (2*j+2) * (2*j+3);
         denom = _mm256_mul_ps(denom, _mm256_set1_ps((2*j+2) * (2*j+3)));
         sign *= -1;
         }
        _mm256_store_ps(&result[i], value);
  }
end_t = clock();
printf("end_t = %ld\n", end_t);
double total_t = (end_t - start_t);
total_t= total_t/ CLOCKS_PER_SEC;
printf("Total time by vector program: %f\n", total_t);
}



int main()
{

	int N=1000000; 
	int terms= 5; 
	float *x= (float*) malloc(N * sizeof(float));
    float *result= (float*) malloc(N * sizeof(float));

	for(int i = 0; i < N; i++){
		x[i] = float(i);}


sinx_scalar( N,  terms, x, result);

sinx_intrinsic (N, terms, x, result); 

//cout<< result[2]; 
getchar();
return 0; 
}