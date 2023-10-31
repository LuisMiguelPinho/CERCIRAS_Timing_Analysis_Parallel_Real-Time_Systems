/*
MIT License

Copyright (c) 2023 Instituto Superior de Engenharia do Porto

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/**
 * Matrix multiplication: A[L,M]* B[M,N]
 **/

#define L 1024
#define M 1024
#define N 1024
#define MIN_RAND -10
#define MAX_RAND 10
#define BS = 64;

int A[L][M];
int B[M][N];
int C[L][N];
int correct_C[L][N];

void fill(int* matrix, int height,int width);
void print(int* matrix,int height,int width);
void setup_correct_C();
void assert(int C[L][N],int expected[L][N]);
void c_clean();

/**
 * Example of a sequential matrix multiplication
*/
void seq()
{
    for (int l = 0; l < L; l++)
    {
        for (int n = 0; n < N; n++)
        {
            int sum = 0;
            for (int m = 0; m < M; m++)
            {
                sum += A[l][m] * B[m][n];
            }
            C[l][n] = sum;
        }
    }
}

void par()
{
    /*TBC*/
}



int main(int argc, char *argv[])
{
    
    srand(time(NULL));
    //Fill A and B with random ints
    fill((int *)A,L,M);
    fill((int *)B,M,N);
    
    //Run sequential version to compare time 
    //and also to have the correct result
    double begin = omp_get_wtime();
    seq();
    double end = omp_get_wtime();
    double sequential_time = end - begin;
    //save correct result in global variable 'correct_C'
    setup_correct_C();
    
    //Clear results in C (i.e. fill with zeros)
    c_clean();

    //invoke your parallel code here. it should be very similar to the previous code
    //e.g.:
    begin = omp_get_wtime();
    par();
    end = omp_get_wtime();
    double parallel_time = end - begin;

    //compare your result invoking the following code (just uncomment the code):
    assert(C,correct_C);

    printf("\n- ==== Performance ==== -\n");
    printf("Sequential time: %fs\n",sequential_time);
    printf("Parallel   time: %fs\n",parallel_time);
    //add a line here printing the results of your version
}


void fill(int* matrix, int height,int width){
    for (int l = 0; l < height; l++)
    {
        for (int n = 0; n < width; n++)
        {
            *((matrix+l*width) + n) = MIN_RAND + rand()%(MAX_RAND-MIN_RAND+1);
        }
    }
}

void print(int* matrix,int height,int width){
    
    for (int l = 0; l < height; l++)
    {
        printf("[");
        for (int n = 0; n < width; n++)
        {
            printf(" %5d",*((matrix+l*width) + n));
        }
        printf(" ]\n");
    }
}

void assert(int C[L][N],int expected[L][N]){
    for (int l = 0; l < L; l++)
    {
        for (int n = 0; n < N; n++)
        {
            if(C[l][n] != expected[l][n]){
                printf("Wrong value at position [%d,%d], expected %d, but got %d instead\n",l,n,expected[l][n],C[l][n]);
                exit(-1);
            }
        }
        
    }
}

void c_clean(){
    for (int l = 0; l < L; l++)
    {
        for (int n = 0; n < N; n++)
        {
            C[l][n] = 0;
        }
    }
}


void setup_correct_C(){
    
    for (int l = 0; l < L; l++)
    {
        for (int n = 0; n < N; n++)
        {
            correct_C[l][n] = C[l][n];
        }
    }
}
