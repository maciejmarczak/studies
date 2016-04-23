#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <limits>
#include <cmath>

using namespace std;


/* Constants and global variables. */
const int N = 22;

double X[N];
double Y[N];

double** create_matrix(int size) {
	double ** matrix = new double* [size];
	for(int i=0; i<size; i++)
		matrix[i] = new double [size];
	return matrix;
}



void print_vector(double* vector) {
	for(int i=0; i<N; i++)
		cout << setprecision(5) << vector[i] << ", ";
	cout << endl << endl;
}

double calculate_derivative(double x) {
	return x + 20 * sin(2 * x);
}

void print_matrix(double** matrix, int size) {
	for(int row=0; row<size; row++) {
		for(int column=0; column<size; column++)
			cout << setprecision(5) << setw(12) << matrix[row][column] << " ";
		cout << endl;
	}
	cout << endl;
}

void fill_Hermite_differences(double** differences) {
	for(int i=0; i<=2*N; i++) {
		for(int j=0; j<=2*N; j++)
			differences[i][j] = 0;
	}

	for(int i=0; i<2*N; i+=2) {
		differences[i][0] = X[i/2];
		differences[i+1][0] = X[i/2];
		
		differences[i][1] = Y[i/2];
		differences[i+1][1] = Y[i/2];
	}
	
	for(int i=2; i<=2*N; i++) {
		for(int j=1+(i-2); j<2*N; j++) {
			if(i == 2 && j%2 == 1)
				differences[j][i] = calculate_derivative(X[j/2]);
			else
				differences[j][i] = (differences[j][i-1] - differences[j-1][i-1]) / (differences[j][0] - differences[(j-1)-(i-2)][0]);
		}
	}
}

double* create_vector() {
	double* vector = new double [N];
	return vector;
}

/* Newton polynomial functions. */
void fill_differences(double** differences) {
	for(int i=0; i<=N; i++) {
		for(int j=0; j<=N; j++)
			differences[i][j] = 0;
	}

	for(int i=0; i<N; i++) {
		differences[i][0] = X[i];
		differences[i][1] = Y[i];
	}

	for(int i=2; i<=N; i++) {
		for(int j=1+(i-2); j<N; j++)
			differences[j][i] = (differences[j][i-1] - differences[j-1][i-1]) / (X[j] - X[(j-1)-(i-2)]);
	}
}

double calculate_Hermite(double x) {
	double** differences = create_matrix(2*N + 1);
	double outcome = 0, factor;

	fill_Hermite_differences(differences);

	for(int i=0; i<2*N; i++) {
		factor = 1;
		for(int j=0; j<i; j++) {
			factor *= (x - X[j/2]);
			if(j+1 != i) {
				factor *= (x - X[j/2]);
				j++;
			}
		}

		outcome += factor * differences[i][i+1];
	}

	return outcome;
}


double calculate_Newton(double x) {
	double** differences = create_matrix(N+1);
	double outcome = 0, factor;

	fill_differences(differences);

	for(int i=0; i<N; i++) {
		factor = 1;
		for(int j=0; j<i; j++)
			factor *= (x - X[j]);

		outcome += factor * differences[i][i+1];
	}

	return outcome;
}



/* Lagrange polynomial functions. */
double Lagrange_coefficient(double x, int i) {
	double outcome = 1;

	for(int j=0; j<N; j++) {
		if(j == i)
			continue;
		outcome *= (x - X[j]) / (X[i] - X[j]);
	}

	return outcome;
}



double calculate_Lagrange(double x) {
	double outcome = 0;

	for(int i=0; i<N; i++)
		outcome += Y[i] * Lagrange_coefficient(x, i);

	return outcome;
}


/* Other functions. */
double calculate_function(double x) {
	return 10 + (x * x) / 2 - 10 * cos(2 * x);
}



void constant_points(double start, double end) {
	double step = (end-start) / (N-1);
	for(int i=0; i<N; i++) {
		X[i] = start + i*step;
		Y[i] = calculate_function(X[i]);
	}
}



void Chebyshev_points(double start, double end) {
	double half_length = (end-start)/2, middle = start + half_length;

	for(int i=0; i<N; i++) {
		X[i] = middle +  cos((double)((2*i+1)*M_PI)/(2*N)) * half_length;
		Y[i] = calculate_function(X[i]);
	}
}



/* Main function. */
int main() {
	double start = -2*M_PI, end = 2*M_PI, step = (end-start)/500, temp, error_Newton, error_Lagrange, error_Hermite, result_function, result_Lagrange, result_Newton, result_Hermite;
	//constant_points(start, end);
	Chebyshev_points(start, end);
	
	ofstream node_file;
	node_file.open("nodes.txt");
	for(int i=0; i<N; i++)
		node_file << X[i] << " " << Y[i] << endl;
	node_file.close();
	
	temp = start;
	error_Newton = 0;
	error_Hermite = 0;
	error_Lagrange = 0;

	double error_ls_Newton = 0.0;
	double error_ls_Lagrange = 0.0;
	double error_ls_Hermite = 0.0;

	while(temp <= end) {

		/*result_function = calculate_function(temp);
		result_Lagrange = calculate_Lagrange(temp);
		result_Newton = calculate_Newton(temp);

		double abs_Lagrange = abs(result_function - result_Lagrange);
		double abs_Newton = abs(result_function - result_Newton);
		if(abs_Lagrange > error_Lagrange) {
			error_Lagrange = abs_Lagrange;
		}
		if(abs_Newton > error_Newton) {
			error_Newton = abs_Newton;
		}

		error_ls_Newton += abs_Newton * abs_Newton;
		error_ls_Lagrange += abs_Lagrange * abs_Lagrange;

		cout << temp << " " << result_function << " " << result_Newton << " " << result_Lagrange << endl;*/

		result_function = calculate_function(temp);
		result_Hermite = calculate_Hermite(temp);

		double abs_Hermite = abs(result_function - result_Hermite);
		
		if(abs_Hermite > error_Hermite)
			error_Hermite = abs_Hermite;

		error_ls_Hermite += abs_Hermite * abs_Hermite;
		
		cout << temp << " " << result_function << " " << result_Hermite << endl;

		temp += step;
	}

	//cout << "ERROR MAX: error Newton: " << error_Newton << endl << "error Lagrange: " << error_Lagrange << endl << endl;
	//cout << "ERROR LSQ: error Newton: " << sqrt(error_ls_Newton)/N << endl << "error Lagrange: " << sqrt(error_ls_Lagrange)/N << endl << endl;
	cout << "error MAX Hermite: " << error_Hermite << endl;
	cout << "error LSQ Hermite: " << sqrt(error_ls_Hermite)/N << endl;

	return 0;
}
