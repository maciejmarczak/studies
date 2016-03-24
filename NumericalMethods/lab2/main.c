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
    double m = 0.5;
    double k = 6.0;

    int n = A->rows;

    int i, j;
    for(i = 0; i < n; i++) {
	for(j = 0; j < n; j++) {
	    if(i == j) {
		A->cells[i][j] = k;
	    }
	    else if(j > i) {
		A->cells[i][j] = pow(-1.0, j + 1.0) * m / (j + 1.0);
	    }
	    else if(i == j + 1) {
		A->cells[i][j] = m / (i + 1.0);
	    }
	    else if(j < i - 1) {
		A->cells[i][j] = 0;
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
            A->cells[i][j] = 0.5;
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

    //printf("X - generated:\n");
    //printMatrixAsVector(X_GEN);
    //printf("X - calculated:\n");
    //printMatrixAsVector(X_CAL);

    printf("Norm of vector X_GEN - X_CAL = %.30e\n\n\n", getEuclideanNorm(X_GEN, X_CAL));
}

Matrix* getIterationMatrix(Matrix* A) {
    int n = A->rows;

    Matrix* RES = createMatrix(n, n);
    
    Matrix* D = createMatrix(n, n);
    Matrix* R = createMatrix(n, n);

    int i, j;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(i == j) {
                D->cells[i][j] = A->cells[i][j];
                R->cells[i][j] = 0.0;
            } else {
                D->cells[i][j] = 0.0;
                R->cells[i][j] = A->cells[i][j];
            }
        }
    }

    // inverse D
    for(i = 0; i < n; i++) {
        D->cells[i][i] = 1.0 / D->cells[i][i];
    }

    RES = multiplyMatrices(D, R);

    return RES;
}

Matrix* getH_i(Matrix* A, int it) {
    int n = A->rows;

    double I[n][n];

    // diagonal matrix
    int i, j;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(i == j){
                I[i][j] = 1.0;
            }
            else {
                I[i][j] = 0.0;
            }
        }
    }

    double V[n];
    double sum = 0.0;

    for(i = 0; i < n; i++) {
        V[i] = A->cells[i][it];

        if(i == it) {
            for(j = 0; j < n; j++) {
                sum += pow(A->cells[j][it], 2);
            }

            V[i] += sqrt(sum);
        }
    }

    double VTV = 0.0;
    for(i = 0; i < n; i++) {
        VTV += V[i] * V[i];
    }

    double VVT[n][n];
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            VVT[i][j] = V[i] * V[j];
        }
    }

    Matrix* H = createMatrix(n, n);

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            H->cells[i][j] = I[i][j] - (2.0 / VTV) * VVT[i][j];
        }
    }

    return H;
}

Matrix* qrMethod(Matrix* A) {
    Matrix* QR = malloc(2 * sizeof(Matrix));

    int n = A->rows;
    QR[0].rows = QR[0].columns = QR[1].rows = QR[1].columns = n;

    QR[0].cells = malloc(n * sizeof(double*));
    QR[1].cells = malloc(n * sizeof(double*));

    int i;
    for(i = 0; i < n; i++) {
        QR[0].cells[i] = malloc(n * sizeof(double));
        QR[1].cells[i] = malloc(n * sizeof(double));
    }

    QR[0] = *(getH_i(A, 0));
    QR[1] = *(getH_i(A, n - 1));

    for(i = 1; i < n; i++) {
        QR[0] = *(multiplyMatrices(&(QR[0]), getH_i(A, i)));
        QR[1] = *(multiplyMatrices(&(QR[1]), getH_i(A, n - i - 1)));
    }

    QR[1] = *(multiplyMatrices(&(QR[1]), A));

    return QR;
}

double getMatrixTopEigenvalue(Matrix* A) {
    int n = A->rows;
    Matrix* A_K1 = createMatrix(n, n);

    int i, j;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            A_K1->cells[i][j] = A->cells[i][j];
        }
    }

    int k;
    for(k = 0; k < 5; k++) {
        Matrix* QR = qrMethod(A_K1);
        A_K1 = multiplyMatrices(&(QR[1]), &(QR[0]));

        for(i = 0; i < n; i++) {
            free(QR[0].cells[i]);
            free(QR[1].cells[i]);
        }

        free(QR[0].cells);
        free(QR[1].cells);
        free(QR);
    }

    double result = 0.0;
    for(i = 0; i < A_K1->rows; i++) {
        if(fabs(A_K1->cells[i][i]) > result) {
            result = fabs(A_K1->cells[i][i]);
        }
    }

    freeMatrixMemory(A_K1);

    return result;
}

Matrix* jacobMethod(Matrix* A, Matrix* B, Matrix* X0, Matrix* X_GEN) {
    Matrix* RES = createMatrix(X0->rows, X0->columns);
    int n = A->rows;
    double sum;
    int steps = 0;

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
        if(getEuclideanNorm(X_GEN, RES) < 5e-10) {
            break;
        }

        // b
        /*if(getEuclideanNorm(multiplyMatrices(A, X0), B) < 5e-10) {
            break;
        }*/

        steps++;
    }


    printf("Number of steps: %d.\n", steps);
    return RES;
}



Matrix* sorMethod(Matrix* A, Matrix* B, Matrix* X0, Matrix* X_GEN) {
    double w = 1.0001;

    Matrix* RES = createMatrix(X0->rows, X0->columns);
    double sum;
    int steps = 0;
    int n = A->rows;

    int i;
    for(i = 0; i < X0->rows; i++) {
        RES->cells[i][0] = X0->cells[i][0];
    }

    while(1) {

        int i, j;
        for(i = 0; i < n; i++) {
            sum = 0.0;
            for(j = 0; j < n; j++) {
                if(j != i) {
                    sum += A->cells[i][j] * RES->cells[j][0];
                }
            }

            RES->cells[i][0] += w * (((B->cells[i][0] - sum) / A->cells[i][i]) - RES->cells[i][0]);
        }

        // a
        if(getEuclideanNorm(X_GEN, RES) < 5e-10) {
            break;
        }

        // b
        /*if(getEuclideanNorm(multiplyMatrices(A, X0), B) < 5e-10) {
            break;
        }*/

        steps++;
    }

    printf("Number of steps: %d.\n", steps);
    return RES;
}

void taskOne() {

    int i;
    for(i = 10; i < 5001; i++) {
	if(!(i == 10 || i  == 50 || i == 100 || i == 5000)) continue;

        Matrix* A = createMatrix(i, i);
        fillWithFirstEquation(A);
        printf("Matrix top eigenvalue: %6.4f\n", getMatrixTopEigenvalue(getIterationMatrix(A)));

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

void taskThree() {

    int i;
    for(i = 10; i < 5001; i++) {
    if(!(i == 10 || i  == 50 || i == 500 || i == 5000)) continue;

        Matrix* A = createMatrix(i, i);
        fillWithFirstEquation(A);

        Matrix* X_GEN = createMatrix(i, 1);
        fillWithZeroOnePermutation(X_GEN);

        Matrix* B = multiplyMatrices(A, X_GEN);

        Matrix* X0 = createMatrix(i, 1);
        fillWithZeros(X0);

        Matrix* X_CAL = sorMethod(A, B, X0, X_GEN);

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
    //taskThree();

    return 0;
}

