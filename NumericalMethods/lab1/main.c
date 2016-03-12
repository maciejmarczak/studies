#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    float** cells;
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
    A->cells = malloc(A->rows * sizeof(float*));

    int i;
    for(i = 0; i < A->rows; i++) {
        A->cells[i] = malloc(A->columns * sizeof(float));
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
            A->cells[i][j] = (float) (rand() % limit);
        }
    }
}

void fillWithFirstEquation(Matrix* A) {
    if(A->rows != A->columns) {
        printf("Wrong matrices size.\n");
        return;
    }

   int n = A->rows;

   int i, j;
   for(i = 0; i < n; i++) A->cells[0][i] = 1;

   for(i = 1; i < n; i++) {
        for(j = 0; j < n; j++) {
            A->cells[i][j] = 1.0 / (i + 1.0 + j);
        }
   }
}

void fillWithSecondEquation(Matrix* A) {
    if(A->rows != A->columns) {
        printf("Wrong matrices size.\n");
        return;
    }

    int n = A->rows;

    int i, j;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(j >= i) {
                A->cells[i][j] = 2.0 * (i + 1.0) / (j + 1.0);
            } else {
                A->cells[i][j] = A->cells[j][i];
            }
        }
    }
}

void fillWithThirdEquation(Matrix* A) {
    int m = 3;
    int k = 4;

    if(A->rows != A->columns) {
        printf("Wrong matrices size.\n");
        return;
    }

    int n = A->rows;

    int i, j;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(i == j) {
                A->cells[i][j] = k;
            }
            else if(i + 1 == j) {
                A->cells[i][j] = 1.0 / (i + 1.0 + m);
            }
            else if(i == j + 1) {
                A->cells[i][j] = k / (i + 2.0 + m);
            }
            else {
                A->cells[i][j] = 0.0;
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

void gaussianElimination(Matrix* A, Matrix* B) {
    float z;
    int i, j, k;
    for(k = 0; k < A->rows - 1; k++) {
        for(i = k + 1; i < A->rows; i++) {
            z = A->cells[i][k] / A->cells[k][k];
            A->cells[i][k] = 0.0;
            for(j = k + 1; j < A->rows; j++) {
                A->cells[i][j] = A->cells[i][j] - (z * (A->cells[k][j]));
            }
            B->cells[i][0] = B->cells[i][0] - (z * (B->cells[k][0]));
        }
    }
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

    float tmp;
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
        float partSum = B->cells[i][0];

        for(j = i + 1; j < A->columns; j++) {
            partSum -= A->cells[i][j] * X->cells[j][0];
        }

        X->cells[i][0] = partSum / A->cells[i][i];
    }

    return X;
}

Matrix* createMatrix(int rows, int columns) {
    Matrix* M = malloc(sizeof(Matrix));
    M->rows = rows;
    M->columns = columns;
    allocateMatrixCellsMemory(M);
    return M;
}

float getEuclideanNorm(Matrix* M1, Matrix* M2) {
    if(!((M1->rows == M2->rows) && M1->columns == 1 && M2->columns == 1)) {
        printf("Couldn't calculate norm.\n");
        return;
    }

    int n = M1->rows;

    float norm = 0.0;

    int i;
    for(i = 0; i < n; i++) {
        norm += pow(M1->cells[i][0] - M2->cells[i][0], 2.0);
    }

    return sqrt(norm);
}

void printNormInformation(int i, Matrix* X_GEN, Matrix* X_CAL) {
    printf("N = %d\n", i);
    /*printf("X - generated:\n");
    printMatrixAsVector(X_GEN);
    printf("X - calculated:\n");
    printMatrixAsVector(X_CAL);
*/
    printf("Norm of vector X_GEN - X_CAL = %.30e\n\n\n", getEuclideanNorm(X_GEN, X_CAL));
}

void taskOne() {
    // AX = B | X_GEN - generated at the beginning, X_CAL - calculated
    Matrix *A, *B, *X_GEN, *X_CAL;

    int i;
    for(i = 5; i < 6; i++) {
        A = createMatrix(i, i);
        fillWithFirstEquation(A);

        printMatrix(A);

        X_GEN = createMatrix(i, 1);
        fillWithZeroOnePermutation(X_GEN);

        B = multiplyMatrices(A, X_GEN);
        gaussianElimination(A, B);
        X_CAL = solveTriangularUpper(A, B);

        printNormInformation(i, X_GEN, X_CAL);

        freeMatrixMemory(A);
        freeMatrixMemory(B);
        freeMatrixMemory(X_GEN);
        freeMatrixMemory(X_CAL);
    }
}

void taskTwo(int variant) {
    // AX = B | X_GEN - generated at the beginning, X_CAL - calculated
    Matrix *A, *B, *X_GEN, *X_CAL;

    int i;
    for(i = 5; i <= 2000; i++) {
        if(!(i == 5 || i == 50 || i == 100 || i == 500 || i == 1000 || i == 2000)) continue;
        A = createMatrix(i, i);
        if(variant == 0) {
            fillWithSecondEquation(A);
        }
        else {
            fillWithThirdEquation(A);
        }

        //printMatrix(A);

        X_GEN = createMatrix(i, 1);
        fillWithZeroOnePermutation(X_GEN);

        B = multiplyMatrices(A, X_GEN);

        time_t before = clock();

        gaussianElimination(A, B);
        X_CAL = solveTriangularUpper(A, B);

        time_t after = clock();

        printf("%10.8f\n", ((float) (after - before) * 1000.0) / CLOCKS_PER_SEC);

        printNormInformation(i, X_GEN, X_CAL);

        freeMatrixMemory(A);
        freeMatrixMemory(B);
        freeMatrixMemory(X_GEN);
        freeMatrixMemory(X_CAL);
    }
}

void taskThree() {

    printf("\n>>>>>>>>>>>>>>>>>>>>   GAUSSIAN   <<<<<<<<<<<<<<<<<<<<\n\n\n\n");
    taskTwo(1);

    Matrix *A, *B, *X_GEN, *X_CAL;
    Matrix *AA, *AB, *AC;

    printf("\n>>>>>>>>>>>>>>>>>>  SPARSE  METHOD  <<<<<<<<<<<<<<<<\n\n\n\n");

    int i;
    for(i = 5; i <= 2000; i++) {
        if(!(i == 5 || i == 50 || i == 100 || i == 500 || i == 1000 || i == 2000)) continue;
        A = createMatrix(i, i);
        fillWithThirdEquation(A);

        AA = createMatrix(i, 1);
        AB = createMatrix(i, 1);
        AC = createMatrix(i, 1);

        int j;
        for(j = 0; j < i; j++) {
            AA->cells[j][0] = A->cells[(j + 1) % i][j];
            AB->cells[j][0] = A->cells[j][j];
            AC->cells[j][0] = A->cells[j][(j + 1) % i];
        }

        X_GEN = createMatrix(i, 1);
        fillWithZeroOnePermutation(X_GEN);
        B = multiplyMatrices(A, X_GEN);

        X_CAL = createMatrix(i, 1);

        time_t before = clock();

        int k;
        for(k = 1; k < i; k++) {
            AB->cells[k][0] -= (AA->cells[k - 1][0] / AB->cells[k - 1][0]) * AC->cells[k - 1][0];
            B->cells[k][0] -= (AA->cells[k - 1][0] / AB->cells[k - 1][0]) * B->cells[k - 1][0];
        }

        X_CAL->cells[i - 1][0] = B->cells[i - 1][0] / AB->cells[i - 1][0];

        for(k = i - 2; k >= 0; k--) {
            X_CAL->cells[k][0] = (B->cells[k][0] - AC->cells[k][0] * X_CAL->cells[k + 1][0]) / AB->cells[k][0];
        }

        time_t after = clock();

        printf("%10.8f\n", ((float) (after - before) * 1000.0) / CLOCKS_PER_SEC);
        printNormInformation(i, X_GEN, X_CAL);

        freeMatrixMemory(X_GEN);
        freeMatrixMemory(X_CAL);
        freeMatrixMemory(AA);
        freeMatrixMemory(AB);
        freeMatrixMemory(AC);
        freeMatrixMemory(B);
        freeMatrixMemory(A);
    }
}

int main(int argc, char** argv) {
    int matrixSize = getFromCommandLine(argc, argv);

    //printf("\n====================   TASK ONE   ====================\n\n\n\n");
    //taskOne();
    //printf("\n====================   TASK TWO   ====================\n\n\n\n");
    //taskTwo(0);
    printf("\n====================  TASK THREE  ====================\n\n\n\n");
    taskThree();

    return 0;
}

