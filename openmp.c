#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ROWS 750
#define COLS 600
#define INNER_DIM 650

int main() {
    int (*matrixA)[COLS] = malloc(sizeof(int[ROWS][COLS]));
    int (*matrixB)[INNER_DIM] = malloc(sizeof(int[COLS][INNER_DIM]));
    int (*resultMatrix)[INNER_DIM] = malloc(sizeof(int[ROWS][INNER_DIM]));

    srand(time(NULL));
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            matrixA[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < INNER_DIM; j++) {
            matrixB[i][j] = rand() % 10;
        }
    }

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(4)
    {
        #pragma omp for
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < INNER_DIM; j++) {
                int sum = 0;
                for (int k = 0; k < COLS; k++) {
                    sum += matrixA[i][k] * matrixB[k][j];
                }
                resultMatrix[i][j] = sum;
            }
        }
    }

    double end_time = omp_get_wtime();
    double elapsed_seconds = end_time - start_time;
    
    printf("Matrix multiplication completed successfully in %f seconds.\n", elapsed_seconds);

    free(matrixA);
    free(matrixB);
    free(resultMatrix);

    return 0;
}
