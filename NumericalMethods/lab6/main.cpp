#include <iostream>
#include <cmath>
#include <iostream>
#include <fstream>

// metoda rozwiązywania w sprawozdaniu

using namespace std;

// polynomial degree and number of points
const int polynomialDegree = 50;
const int N = 50;

double A[ polynomialDegree + 1 ];

double function_value(double x) {
	return 10 + pow(x, 2) / 2 - 10 * cos(2 * x);
}

double approx_value(double x) {
	double res = 0.0;

	int i;
	for (i = 0; i <= polynomialDegree; i++) {
		res += A[i] * pow(x, i);
	}

	return res;
}

void gaussianElimination(double A[][ polynomialDegree + 1 ], double *B) {
    double z;
    int i, j, k;
    for(k = 0; k <= polynomialDegree; k++) {
        for(i = k + 1; i <= polynomialDegree; i++) {
            z = A[i][k] / A[k][k];
            A[i][k] = 0.0;
            for(j = k + 1; j <= polynomialDegree; j++) {
                A[i][j] = A[i][j] - (z * A[k][j]);
            }
            B[i] = B[i] - (z * B[k]);
        }
    }
}

void solveTriangularUpper(double A[][ polynomialDegree + 1 ], double *B, double *RES) {
    int i, j;
    for(i = polynomialDegree; i >= 0; i--) {
        double partSum = B[i];

        for(j = i + 1; j <= polynomialDegree; j++) {
            partSum -= A[i][j] * RES[j];
        }

        RES[i] = partSum / A[i][i];
    }
}

int main(int argc, char **argv) {

	double START = - 2 * M_PI, END = 2 * M_PI;

	double T[ polynomialDegree + 1 ];
	double S[ polynomialDegree + 1 ][ polynomialDegree + 1 ];

	double X[N], Y[N];
	double jump = (END - START) / (N - 1);

	ofstream myfile;
	myfile.open ("nodes.txt");

	for (int i = 0; i < N; i++) {
		X[i] = START + i * jump;
		Y[i] = function_value(X[i]);
		
		myfile << X[i] << " " << Y[i] << endl;
	}

	myfile.close();	

	int i, j, k;
	for (i = 0; i <= polynomialDegree; i++) {
		for (j = 0; j <= polynomialDegree; j++) {
			S[i][j] = 0.0;
			for (k = 0; k < N; k++) {
				S[i][j] += pow(X[k], i + j);
			}
		}

		T[i] = 0.0;

		for (k = 0; k < N; k++) {
			T[i] += Y[k] * pow(X[k], i);
		}

	}

	gaussianElimination(S, T);
	solveTriangularUpper(S, T, A);

	double plot_step = (END - START) / 500, curr = START;
	double abs_err = 0.0;
	double ls_err = 0.0;

	while (curr <= END) {
		double app_val = approx_value(curr);
		double fun_val = function_value(curr);

		double abs_val = abs(fun_val - app_val);

		if (abs_val > abs_err) abs_err = abs_val;

		ls_err += pow(abs_val, 2);

		cout << curr << " " << app_val << " " << fun_val << endl;
		curr += plot_step;
	}

	cout << "abs_err: " << abs_err << "\tls_err: " << ls_err / N << endl;

	return 0;
}