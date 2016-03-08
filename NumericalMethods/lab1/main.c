#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    double** cells;
    int rows;
    int columns;
} Matrix;

int getFromCommandLine(int argc, char** argv) {
    // default value
    int size = 5;

    if(argc == 0) {
        return size;
    }

    int commandLineVal = atoi(argv[0]);
    if(commandLineVal > 3 && commandLineVal < 20) {
        size = commandLineVal;
    }

    return size;
}

void allocateMatrixCellsMemory(Matrix* A) {
    A->cells = malloc(A->rows * sizeof(double*));

    int i;
    for(i = 0; i < A->rows; i++) {
        A->cells[i] = malloc(A->columns * sizeof(double));
    }
}

void freeMatrixMemory(Matrix* A) {
    int i;
    for(i = 0; i < A->rows; i++) {
        free(A->cells[i]);
    }

    free(A->cells);
    free(A);
}

void fillWithRandomValues(Matrix* A, int limit) {
    srand(time(NULL));

    int i, j;
    for(i = 0; i < A->rows; i++) {
        for(j = 0; j < A->columns; j++) {
            A->cells[i][j] = (double) (rand() % limit);
        }
    }
}

void printMatrix(Matrix* A) {
    int i, j;
    for(i = 0; i < A->rows; i++) {
        printf("|");
        for(j = 0; j < A->columns; j++) {
            printf("%8.2f", A->cells[i][j]);
        }
        printf(" |\n");
    }

    printf("\n\n");
}

void gaussianElimination(Matrix* A) {
    double z;
    int i, j, k;
    for(k = 0; k < A->rows - 1; k++) {
        for(i = k + 1; i < A->rows; i++) {
            z = A->cells[i][k] / A->cells[k][k];
            A->cells[i][k] = 0.0;
            for(j = k + 1; j < A->rows; j++) {
                A->cells[i][j] = A->cells[i][j] - z * A->cells[k][j];
            }
        }
    }
}

Matrix* multiplyMatrices(Matrix* A, Matrix* B) {
    // check if can be multiplied
    if(A->columns != B->rows) {
        printf("Cannot multiply given matrices.\n");
        return NULL;
    }

    Matrix* C = malloc(sizeof(Matrix));
    C->rows = A->rows;
    C->columns = B->columns;
    allocateMatrixCellsMemory(C);

    double tmp;
    int i, j, k;
    for(i = 0; i < C->rows; i++) {
        for(j = 0; j < C->columns; j++) {
            tmp = 0.0;
            for(k = 0; k < A->columns; k++) {
                tmp += A->cells[i][k] * B->cells[k][j];
            }
            C->cells[i][j] = tmp;
        }
    }

    return C;
}

Matrix* solveTriangularUpper(Matrix* A, Matrix* B) {
    Matrix* X = malloc(sizeof(Matrix));
    X->rows = A->columns;
    X->columns = B->columns;
    allocateMatrixCellsMemory(X);

    int i, j;
    for(i = A->columns - 1; i >= 0; i--) {
        double partSum = B->cells[i][0];

        for(j = i + 1; j < A->columns - 1; j++) {
            partSum -= A->cells[i][j] * X->cells[j][0];
        }

        X->cells[i][0] = partSum / A->cells[i][i];
    }

    return X;
}

int main(int argc, char** argv) {
    int matrixSize = getFromCommandLine(argc, argv);

    Matrix* A = malloc(sizeof(Matrix));
    A->rows = matrixSize;
    A->columns = matrixSize;

    allocateMatrixCellsMemory(A);
    fillWithRandomValues(A, 50);
    printMatrix(A);

    Matrix* B = malloc(sizeof(Matrix));
    B->rows = 5;
    B->columns = 1;

    allocateMatrixCellsMemory(B);
    fillWithRandomValues(B, 10);
    printMatrix(B);

    gaussianElimination(A);
    Matrix* X = solveTriangularUpper(A, B);
    printMatrix(X);

    Matrix* C = multiplyMatrices(A, X);
    printMatrix(C);

    freeMatrixMemory(A);
    freeMatrixMemory(B);
    freeMatrixMemory(X);
    freeMatrixMemory(C);

    return 0;
}
