#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <limits>
#include <cmath>
#include <ctime>

int X_mesh = 11, T_mesh = 351;

using namespace std;

double* create_vector(int size) {
	return new double[size];
}

double** create_matrix(int rows, int columns) {
	double **matrix = new double*[rows];
	for (int i = 0; i < rows; i++) matrix[i] = new double[columns];
	return matrix;
}

double **create_tridiagonal(int size) {
	double **matrix = new double*[size];
	for (int i = 0; i < size; i++) matrix[i] = new double[3];
	return matrix;
}

void Thomas_algorithm(double **matrix, double *vector, int size) {
	for (int i = 0; i < size; i++) {
		double divisor = matrix[i][1];
		
		for (int j = 0; j < 3; j++)
			matrix[i][j] /= divisor;
		vector[i] /= divisor;
		
		if(i != size-1) {
			double factor = matrix[i + 1][0];
			for(int j = 0; j < 2; j++)
				matrix[i + 1][j] -= factor * matrix[i][j + 1];
			vector[i + 1] -= factor * vector[i];
		}
	}
	
	/* Backward substitution */
	for (int i = size - 2; i >= 0; i--) {
	    double factor = matrix[i][2];
	    matrix[i][2] = 0;
	    vector[i] -= factor * vector[i + 1];
	}
}

/* Finite differences method for 2-dimensional problem - explicit method. */
void FDM_explicit() {
	const clock_t begin_time = clock();
	double X_start = 0.0, X_end = 3 * M_PI, T_start = 0.0, T_end = 4, a = 1.5;

	double X_step = (X_end - X_start) / (X_mesh - 1);
	double T_step = (T_end - T_start) / (T_mesh - 1);

	double *X_coord = create_vector(X_mesh);
	for (int i = 0; i < X_mesh; i++)
		X_coord[i] = X_start + i * X_step;

	double *T_coord = create_vector(T_mesh);
	for (int i = 0; i < T_mesh; i++)
		T_coord[i] = T_start + i * T_step;

	double **meshgrid = create_matrix(T_mesh, X_mesh);

	/* Applying initial conditions. */
	for (int i = 0; i < X_mesh; i++)
		meshgrid[0][i] = 2 * sin(X_coord[i]);

	/* Applying boundary conditions. */
	for (int i = 0; i < T_mesh; i++) {
		meshgrid[i][0] = 0.0;
		meshgrid[i][X_mesh - 1] = 0.0;
	}

	/* Filling meshgrid inside. */
	double coefficient = (pow(a, 2) * T_step) / (pow(X_step, 2));
	for (int j = 1; j < T_mesh; j++) {
		for (int i = 1; i < X_mesh - 1; i++)
			meshgrid[j][i] = coefficient * meshgrid[j-1][i-1] + (1 - 2 * coefficient) * meshgrid[j - 1][i] + coefficient * meshgrid[j - 1][i + 1];
	}
	
	double max_val = 0;
	for (int i = 0; i < X_mesh; i++) {
		for (int j = 0; j < T_mesh; j++) {
			if (abs(meshgrid[j][i]) > max_val)
				max_val = abs(meshgrid[j][i]);
			//cout << X_coord[i] << " " << T_coord[j] << " " << meshgrid[j][i] << endl;
		}
		//cout << endl;
	}

	cout << X_mesh << "\t\t" << T_mesh << "\t\t" << max_val << "\t\t" << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
}

void FDM_implicit() {
	const clock_t begin_time = clock();
	double X_start = 0.0, X_end = 3 * M_PI, T_start = 0.0, T_end = 4, a = 1.5;

	double X_step = (X_end - X_start) / (X_mesh - 1);
	double T_step = (T_end - T_start) / (T_mesh - 1);

	double *X_coord = create_vector(X_mesh);
	for (int i = 0; i < X_mesh; i++)
		X_coord[i] = X_start + i * X_step;

	double *T_coord = create_vector(T_mesh);
	for (int i = 0; i < T_mesh; i++)
		T_coord[i] = T_start + i * T_step;

	double **meshgrid = create_matrix(T_mesh, X_mesh);

	/* Applying initial conditions. */
	for (int i = 0; i < X_mesh; i++)
		meshgrid[0][i] = 2 * sin(X_coord[i]);

	/* Applying boundary conditions. */
	for (int i = 0; i < T_mesh; i++) {
		meshgrid[i][0] = 0.0;
		meshgrid[i][X_mesh - 1] = 0.0;
	}

	/* Filling meshgrid inside. */
	double coefficient = (pow(a, 2) * T_step) / (pow(X_step, 2));
	double *values = create_vector(X_mesh);
	double **matrix = create_tridiagonal(X_mesh);
	
	for (int j = 1; j < T_mesh; j++) {
		for (int k = 1; k < X_mesh - 1; k++) {
			matrix[k][0] = (-1) * coefficient;
			matrix[k][1] = 1 + 2 * coefficient;
			matrix[k][2] = (-1)  *coefficient;
			values[k] = meshgrid[j - 1][k];
		}
		
		matrix[0][0] = 0;
		matrix[0][1] = 1;
		matrix[0][2] = 0;
		matrix[X_mesh - 1][0] = 0;
		matrix[X_mesh - 1][1] = 1;
		matrix[X_mesh - 1][2] = 0;

		values[0] = meshgrid[j][0];
		values[X_mesh - 1] = meshgrid[j][X_mesh - 1];
		
		Thomas_algorithm(matrix, values, X_mesh);
		
		for (int i = 1; i < X_mesh - 1; i++)
			meshgrid[j][i] = values[i];
	}
	
	double max_val = 0;
	for (int i = 0; i < X_mesh; i++) {
		for (int j = 0; j < T_mesh; j++) {
			if (abs(meshgrid[j][i]) > max_val)
				max_val = abs(meshgrid[j][i]);
			//cout << X_coord[i] << " " << T_coord[j] << " " << meshgrid[j][i] << endl;
		}
		//cout << endl;
	}

	cout << X_mesh << "\t\t" << T_mesh << "\t\t" << max_val << "\t\t" << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
}

/* Main function. */
int main() {
	for (X_mesh = 5; X_mesh < 101; X_mesh++) {
		for (T_mesh = 20; T_mesh < 301; T_mesh++) {
			FDM_explicit();
			//FDM_implicit();
		}
		cout << endl;
	}
	
	return 0;
}
