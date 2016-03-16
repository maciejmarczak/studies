#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    double** cells;
    int rows;
    int columns;
} Matrix;

int getFromCommandLine(int argc, char** argv) {
    // default value
    int size = 5;

    if(argc == 1) {
        return size;
    }

    int commandLineVal = atoi(argv[1]);
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

void fillWithFirstEquation(Matrix* A) {
    int m = 4;
    int k = 4;

    int n = A->rows;

    int i, j;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(i == j) {
                A->cells[i][j] = k;
            }
            else {
                A->cells[i][j] = (double) m / (n + i - j + 1.0);
            }
        }
    }
}

void fillWithZeroOnePermutation(Matrix* A) {
    srand(time(NULL));

    int rows = A->rows;
    int columns = A->columns;

    int i, j;
    for(i = 0; i < rows; i++) {
        for(j = 0; j < columns; j++) {
            if(i % 2 == 0) {
                A->cells[i][j] = 1.0;
            } else {
                A->cells[i][j] = -1.0;
            }
        }
    }
}

void fillWithZeros(Matrix* A) {
    int rows = A->rows;
    int columns = A->columns;

    int i, j;
    for(i = 0; i < rows; i++) {
        for(j = 0; j < columns; j++) {
            A->cells[i][j] = 0.0;
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

void printMatrixAsVector(Matrix* A) {
    printf("|");
    int i;
    for(i = 0; i < A->rows; i++) {
        printf("%5.3f  ", A->cells[i][0]);
    }
    printf(" |\n\n");
}

Matrix* multiplyMatrices(Matrix* A, Matrix* B) {
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

Matrix* createMatrix(int rows, int columns) {
    Matrix* M = malloc(sizeof(Matrix));
    M->rows = rows;
    M->columns = columns;
    allocateMatrixCellsMemory(M);
    return M;
}

double getEuclideanNorm(Matrix* M1, Matrix* M2) {
    if(!((M1->rows == M2->rows) && M1->columns == 1 && M2->columns == 1)) {
        printf("Couldn't calculate norm.\n");
        return;
    }

    int n = M1->rows;

    double norm = 0.0;

    int i;
    for(i = 0; i < n; i++) {
        norm += pow(M1->cells[i][0] - M2->cells[i][0], 2.0);
    }

    return sqrt(norm);
}

void printNormInformation(int i, Matrix* X_GEN, Matrix* X_CAL) {
    printf("N = %d\n", i);

    printf("X - generated:\n");
    printMatrixAsVector(X_GEN);
    printf("X - calculated:\n");
    printMatrixAsVector(X_CAL);

    printf("Norm of vector X_GEN - X_CAL = %.30e\n\n\n", getEuclideanNorm(X_GEN, X_CAL));
}

Matrix* jacobMethod(Matrix* A, Matrix* B, Matrix* X0, Matrix* X_GEN) {
    Matrix* RES = createMatrix(X0->rows, X0->columns);
    int n = A->rows;
    double sum;

    int k, i, v;
    for(k = 0; ; k++) {
        for(i = 0; i < n; i++) {

            sum = 0.0;
            for(v = 0; v < n; v++) {
                if(v != i) sum += A->cells[i][v] * X0->cells[v][0];
            }

            RES->cells[i][0] = (double) (B->cells[i][0] - sum) / A->cells[i][i];
        }

        for(i = 0; i < n; i++) {
            X0->cells[i][0] = RES->cells[i][0];
        }

        // a
        /*if(getEuclideanNorm(X_GEN, RES) < 0.000000006) {
            break;
        }*/

        // b
        if(getEuclideanNorm(multiplyMatrices(A, X0), B) < 0.000000006) {
            break;
        }
    }

    return RES;
}

void taskOne() {

    int i;
    for(i = 5; i < 16; i++) {
        Matrix* A = createMatrix(i, i);
        fillWithFirstEquation(A);

        Matrix* X_GEN = createMatrix(i, 1);
        fillWithZeroOnePermutation(X_GEN);

        Matrix* B = multiplyMatrices(A, X_GEN);

        Matrix* X0 = createMatrix(i, 1);
        fillWithZeros(X0);

        Matrix* X_CAL = jacobMethod(A, B, X0, X_GEN);

        printNormInformation(i, X_GEN, X_CAL);

        freeMatrixMemory(A);
        freeMatrixMemory(X_GEN);
        freeMatrixMemory(X_CAL);
        freeMatrixMemory(B);
        freeMatrixMemory(X0);
    }
}

int main(int argc, char** argv) {
    int matrixSize = getFromCommandLine(argc, argv);

    taskOne();

    return 0;
}
