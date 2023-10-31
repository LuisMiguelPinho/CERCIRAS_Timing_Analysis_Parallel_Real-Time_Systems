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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define ARR_LEN 256000 // The length of the array
#define MIN_VAL -60 // Minimum value of the array elements
#define MAX_VAL 125 // Maximum value of the array elements
#define OMP_NUM_THREADS 4

int generate_random(int min, int max);
void quick_sort_par(int array[], int low, int high);
void quick_sort_seq(int array[], int low, int high);
int split(int array[], int low, int high);
void swap_element(int* x, int* y);

/* The main function */
int main(int argc, char *argv[])
{
	int i, array[ARR_LEN], array_copy[ARR_LEN];

	/* Use current time as the seed for random generator */
	srand(time(0));


	//printf("before:");
	/* Generate the elements of the array randomly */
	for (i = 0; i < ARR_LEN; i++){
		array[i] = generate_random(MIN_VAL, MAX_VAL);
        array_copy[i] = array[i];
		//printf(" %d",array[i]);
	}
	printf("\n");

    /* Call the sequential quick_sort function to sort the array */

    double begin = omp_get_wtime();
    quick_sort_seq(array, 0, ARR_LEN - 1);
    double end = omp_get_wtime();
    double sequential_time = end - begin;

    /* Restore the array */
	for (i = 0; i < ARR_LEN; i++){
		array[i] = array_copy[i] ;
	}

    /* Call the parallel quick_sort function to sort the array */

	begin = omp_get_wtime();
    #pragma omp parallel
    #pragma omp single
	quick_sort_par(array, 0, ARR_LEN - 1);

    end = omp_get_wtime();
    double parallel_time = end - begin;

	/*
	printf("after:");
	for (i = 0; i < ARR_LEN; i++){
		printf(" %d",array[i]);
	}
	printf("\n");*/

    printf("\n- ==== Performance ==== -\n");
    printf("Sequential time: %fs\n",sequential_time);
    printf("Parallel   time: %fs\n",parallel_time);
	return 0;
}


/* Generate a random number */
int generate_random(int min, int max) {
	/* Generate a random number between min and max */
	return (rand() % (max - min + 1)) + min;
}

/* The main function to perform the sort operation */

void quick_sort_seq(int array[], int low, int high) {

	/* The main process of the function */
	if (low < high) {
		int m;
		
		/* Specify the middle of the array by calling the split function */
		m = split(array, low, high);
		
		/* Call the quick_sort function to sort the antecedent array */
		quick_sort_seq(array, low, m - 1);
		/* Call the quick_sort function to sort the subsequent array */
		quick_sort_seq(array, m + 1, high);
      
	}
}

void quick_sort_par(int array[], int low, int high) {

	/*TBC*/

}

/* This function takes last element as index, places the index element at
its correct position in sorted array, and places all smaller (smaller than
the index) to left of the index and all greater elements to right of the
index */
int split(int array[], int low, int high) {
	int i, index = array[high];
	int m = low - 1;

	/* The main process of the function */
	for (i = low; i <= high - 1; i++)
		if (array[i] < index) {
			m++;				
			/* Call the swap_element function to swap the elements */
			swap_element(&array[m], &array[i]);
		}

	/* Call the swap_element function to swap the elements */
	swap_element(&array[m + 1], &array[high]);
	return m + 1;
}

/* Swap two elements */
void swap_element(int* x, int* y) {

	/* The main process of the function */
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}