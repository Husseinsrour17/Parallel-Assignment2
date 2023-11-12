#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ROWS 750
#define COLS 600
#define INNER_DIM 650
#define NUM_THREADS 1

struct ThreadArguments {
    int startRow;
    int endRow;
    int (*matrixA)[COLS];
    int (*matrixB)[INNER_DIM];
    int (*matrixC)[INNER_DIM];
};
void* matrixMultiplication(void* argument) {
    struct ThreadArguments* args = (struct ThreadArguments*) argument;
    for (int i = args->startRow; i < args->endRow; i++) {
        for (int j = 0; j < INNER_DIM; j++) {
            int sum = 0;
            for (int k = 0; k < COLS; k++) {
                sum += args->matrixA[i][k] * args->matrixB[k][j];
            }
            args->matrixC[i][j] = sum;
        }
    }
    pthread_exit(NULL);
}
int main() {
    int (*matrixA)[COLS] = malloc(sizeof(int[ROWS][COLS]));
    int (*matrixB)[INNER_DIM] = malloc(sizeof(int[COLS][INNER_DIM]));
    int (*matrixC)[INNER_DIM] = malloc(sizeof(int[ROWS][INNER_DIM]));

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
    clock_t startTime = clock();
    pthread_t threads[NUM_THREADS];
    struct ThreadArguments arguments[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        arguments[i].startRow = i * ROWS / NUM_THREADS;
        arguments[i].endRow = (i + 1) * ROWS / NUM_THREADS;
        arguments[i].matrixA = matrixA;
        arguments[i].matrixB = matrixB;
        arguments[i].matrixC = matrixC;
        pthread_create(&threads[i], NULL, matrixMultiplication, &arguments[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t endTime = clock();

    printf("Matrix multiplication completed successfully.\n");
    printf("Execution time using %d threads: %f seconds.\n", NUM_THREADS, (double)(endTime - startTime) / CLOCKS_PER_SEC);

    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0;
}
