#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include "omp.h"
#include "cholesky.h"

const int  n = 1000; // matrix size
const int ts = 10; // tile size
int num_threads = 4; // number of threads to use

//Parallel For
void cholesky_blocked_par_for(const int ts, const int nt, double* Ah[nt][nt])
{
   
   for (int k = 0; k < nt; k++) {

      // Diagonal Block factorization
      potrf (Ah[k][k], ts, ts);
      // Triangular systems
      #pragma omp parallel for
      for (int i = k + 1; i < nt; i++) {
         trsm (Ah[k][k], Ah[k][i], ts, ts);
      }

      // Update trailing matrix
      for (int i = k + 1; i < nt; i++) {
         #pragma omp parallel for
         for (int j = k + 1; j < i; j++) {
            gemm (Ah[k][i], Ah[k][j], Ah[j][i], ts, ts);
         }
         syrk (Ah[k][i], Ah[i][i], ts, ts);
      }

   }
}

void cholesky_task(int ts, int nt, double* a[nt][nt]) {
   #pragma omp parallel
   #pragma omp single
        for (int k = 0; k < nt; k++) {
                // Diagonal Block factorization
                potrf(a[k][k], ts, ts);
                // Triangular systems
                for (int i = k + 1; i < nt; i++) {
                        #pragma omp task
                        trsm(a[k][k], a[k][i], ts, ts);
                }
                
                #pragma omp taskwait
                // Update trailing matrix
                for (int i = k + 1; i < nt; i++) {
                        for (int j = k + 1; j < i; j++) {
                                #pragma omp task
                                gemm(a[k][i], a[k][j], a[j][i], ts, ts);
                        }
                        #pragma omp task
                        syrk(a[k][i], a[i][i], ts, ts);
                }
                #pragma omp taskwait
        }
}

void cholesky_task_deps(int ts, int nt, double* a[nt][nt]) {

   #pragma omp parallel
   #pragma omp single
        for (int k = 0; k < nt; k++) {
                // Diagonal Block factorization
                #pragma omp task depend(inout: a[k][k])
                potrf(a[k][k], ts, ts);
                // Triangular systems
                for (int i = k + 1; i < nt; i++) {
                        #pragma omp task depend(in: a[k][k]) depend(inout: a[k][i])
                        trsm(a[k][k], a[k][i], ts, ts);
                }
                // Update trailing matrix
                for (int i = k + 1; i < nt; i++) {
                        for (int j = k + 1; j < i; j++) {
                                #pragma omp task depend(inout: a[j][i]) depend(in: a[k][i], a[k][j])
                                gemm(a[k][i], a[k][j], a[j][i], ts, ts);
                        }
                        #pragma omp task depend(inout: a[i][i]) depend(in: a[k][i])
                        syrk(a[k][i], a[i][i], ts, ts);
                }
        }
}


//Sequential
void cholesky_blocked(const int ts, const int nt, double* Ah[nt][nt])
{
   for (int k = 0; k < nt; k++) {
      // Diagonal Block factorization
      potrf (Ah[k][k], ts, ts);

      // Triangular systems
      for (int i = k + 1; i < nt; i++) {
         trsm (Ah[k][k], Ah[k][i], ts, ts);
      }

      // Update trailing matrix
      for (int i = k + 1; i < nt; i++) {
         for (int j = k + 1; j < i; j++) {
            gemm (Ah[k][i], Ah[k][j], Ah[j][i], ts, ts);
         }
         syrk (Ah[k][i], Ah[i][i], ts, ts);
      }
   }
}

void print_matrix(int n, double * const matrix){
   printf("\t{\n\t");
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			printf("%.5f, ",matrix[i*n + j]);
		}
		printf("\n\t");
	}
	printf("};\n");
}

int main(int argc, char* argv[])
{

   omp_set_num_threads(num_threads);
   // Allocate matrix
   double * const matrix = (double *) malloc(n * n * sizeof(double));
   assert(matrix != NULL);

   // Init matrix
   initialize_matrix(n, ts, matrix);

   // Allocate matrix
   double * const original_matrix = (double *) malloc(n * n * sizeof(double));
   assert(original_matrix != NULL);

   // Allocate matrix
   double * const expected_matrix = (double *) malloc(n * n * sizeof(double));
   assert(expected_matrix != NULL);

   const int nt = n / ts;

   // Allocate blocked matrix
   double *Ah[nt][nt];

   for (int i = 0; i < nt; i++) {
      for (int j = 0; j < nt; j++) {
         Ah[i][j] = malloc(ts * ts * sizeof(double));
         assert(Ah[i][j] != NULL);
      }
   }

   for (int i = 0; i < n * n; i++ ) {
      original_matrix[i] = matrix[i];
   }
   // Sequential
   convert_to_blocks(ts, nt, n, (double(*)[n]) matrix, Ah);
   // warming up libraries
   cholesky_blocked(ts, nt, (double* (*)[nt]) Ah);
   convert_to_blocks(ts, nt, n, (double(*)[n]) matrix, Ah);
   // done warming up
   float t1 = get_time();
   //run sequential version
   cholesky_blocked(ts, nt, (double* (*)[nt]) Ah);
   float t2 = get_time() - t1;
   //calculate timing metrics
   const float seq_time = t2;
   float seq_gflops = (((1.0 / 3.0) * n * n * n) / ((seq_time) * 1.0e+9));

   //saving matrix to the expected result matrix
   convert_to_linear(ts, nt, n, Ah, (double (*)[n]) matrix);
   for (int i = 0; i < n * n; i++ ) {
      expected_matrix[i] = matrix[i];
   }
   // End Sequential

   /*************************************************************************************************************
    * NOTE FOR STUDENTS: 
    * COPY the following code (between multiline comments, up to "End Parallel For") to invoke your versio
    * AND make the following changes:
    *  1. change "cholesky_blocked_par_for" to the name of your method
    *  2. change "par_for_time" and "par_for_gflops" for names that reflect your implementation (e.g. par_task_time)
    *     2.1. Don't forget to change the "par_for_time" variable that is used to calculate the gflops
    *  3. add two lines that print the time for your code (below in "Print Result" section)
    *************************************************************************************************************/
    
   /*****************************************************************************************************
    * Parallel For
    *****************************************************************************************************/
   //resetting matrix
   for (int i = 0; i < n * n; i++ ) {
      matrix[i] = original_matrix[i];
   }
   //require to work with blocks
   convert_to_blocks(ts, nt, n, (double(*)[n]) matrix, Ah);
   t1 = get_time();
   //run parallel version using parallel fors
   cholesky_blocked_par_for(ts, nt, (double* (*)[nt]) Ah);
   t2 = get_time() - t1;
   //calculate timing metrics
   float par_for_time = t2;
   float par_for_gflops = (((1.0 / 3.0) * n * n * n) / ((par_for_time) * 1.0e+9));

   //asserting result, comparing the output to the expect matrix
   convert_to_linear(ts, nt, n, Ah, (double (*)[n]) matrix);
   assert_matrix(n,matrix,expected_matrix);

   /*****************************************************************************************************
    * End Parallel For
    *****************************************************************************************************/

/*****************************************************************************************************
    * Parallel Task
    *****************************************************************************************************/
   //resetting matrix
   for (int i = 0; i < n * n; i++ ) {
      matrix[i] = original_matrix[i];
   }
   //require to work with blocks
   convert_to_blocks(ts, nt, n, (double(*)[n]) matrix, Ah);
   t1 = get_time();
   //run parallel version using parallel fors
   cholesky_task(ts, nt, (double* (*)[nt]) Ah);
   t2 = get_time() - t1;
   //calculate timing metrics
   float task_time = t2;
   float task_gflops = (((1.0 / 3.0) * n * n * n) / ((task_time) * 1.0e+9));

   //asserting result, comparing the output to the expect matrix
   // convert_to_linear(ts, nt, n, Ah, (double (*)[n]) matrix);
   // assert_matrix(n,matrix,expected_matrix);

   /*****************************************************************************************************
    * End Parallel Task
    *****************************************************************************************************/

/*****************************************************************************************************
    * Parallel Task with dependencies
    *****************************************************************************************************/
   //resetting matrix
   for (int i = 0; i < n * n; i++ ) {
      matrix[i] = original_matrix[i];
   }
   //require to work with blocks
   convert_to_blocks(ts, nt, n, (double(*)[n]) matrix, Ah);
   t1 = get_time();
   //run parallel version using parallel fors
   cholesky_task_deps(ts, nt, (double* (*)[nt]) Ah);
   t2 = get_time() - t1;
   //calculate timing metrics
   float task_dep_time = t2;
   float task_dep_gflops = (((1.0 / 3.0) * n * n * n) / ((task_dep_time) * 1.0e+9));

   //asserting result, comparing the output to the expect matrix
   convert_to_linear(ts, nt, n, Ah, (double (*)[n]) matrix);
   assert_matrix(n,matrix,expected_matrix);

   /*****************************************************************************************************
    * End Parallel Task with dependencies
    *****************************************************************************************************/


   // Print result
   printf( "============ CHOLESKY RESULTS ============\n" );
   printf( "  matrix size:                  %dx%d\n", n, n);
   printf( "  block size:                   %dx%d\n", ts, ts);
   printf( "  number of threads:            %d\n", num_threads);
   printf( "  seq_time (s):                 %f\n", seq_time);
   printf( "  seq_performance (gflops):     %f\n", seq_gflops);
   printf( "  par_for_time (s):             %f\n", par_for_time);
   printf( "  par_for_performance (gflops): %f\n", par_for_gflops);
   printf( "  task_time (s):                %f\n", task_time);
   printf( "  task_performance (gflops):    %f\n", task_gflops);
   printf( "  task_dep_time (s):            %f\n", task_dep_time);
   printf( "  task_dep_performance (gflops):%f\n", task_dep_gflops);
   printf( "==========================================\n" );


   free(original_matrix);
   free(expected_matrix);
   // Free blocked matrix
   for (int i = 0; i < nt; i++) {
      for (int j = 0; j < nt; j++) {
         assert(Ah[i][j] != NULL);
         free(Ah[i][j]);
      }
   }
   // Free matrix
   free(matrix);

   return 0;
}

