#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

double f_ex(double x) {
	double res = 0.0;

	// zad1
	/*res += exp(-cos(2 * x));
	res -= cos(2 * x);
	res += 1.0;*/

	// zad2
	res = - 2 * sin(4 * x) + 2 * x;

	return res;
}

// example: x' = cost - sinx + t^2 AND x(-1) = 3

double f(double t, double x) {
	return 2 * x * sin(2 * t) + 2 * sin(2 * t) * cos(2 * t);
}

double f_bis(double t, double x, double x_prim) {
	return 32 * t - 16 * x;
}

double ut(double t) {
	return f_bis(t, 0, 0);
}

double vt(double t) {
	return f_bis(t, 1, 0) - f_bis(t, 0, 0);
}

double wt(double t) {
	return f_bis(t, 0, 1) - f_bis(t, 0, 0);
}

double dy(double h, double t, double x) {
	double k1 = f(t, x);
	double k2 = f(t + h / 2, x + k1 * h / 2);
	double k3 = f(t + h / 2, x + k2 * h / 2);
	double k4 = f(t + h, x + k3 * h);

	return h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}

void draw(char method, int steps, double *t, double *x) {
	
	ofstream ofile, points, org_points;

	ofile.open("lab7.plt", ios::in | ios::out | ios::trunc);
	points.open("points.txt", ios::in | ios::out | ios::trunc);
	org_points.open("org_points.txt", ios::in | ios::out | ios::trunc);

	if ( ofile.good() && points.good() ) {

		// zmienic granice w przypadku zadania pierwszego
		double jmp = (M_PI + 1) / 4000;
		for (double i = 0.0; i <= (M_PI + 1) / 2; i += jmp) {
			org_points << i << " " << f_ex(i) << endl;
		}

		double max_err = 0.0;
		for (int i = 0; i < steps; i++) {
			points << t[i] << " " << x[i] << endl;
			if ( abs(x[i] - f_ex(t[i])) > max_err) max_err = abs(x[i] - f_ex(t[i]));
		}

		char euler[] = "Euler", rk[] = "Runge-Kutta", fin[] = "Finite Difference";

		ofile << "set title '" << ((method == 'e') ? euler : (method == 'r') ? rk : fin) << "   max_err: " << max_err << endl;
		ofile << "set xrange [" << t[0] - 1 << ":" << t[steps - 1] + 1 << "]" << endl;
		ofile << "plot 'points.txt' u 1:2 title 'f(x)' w lines lt rgb \"#03A8FF\", \\" << endl;
		ofile << "'org_points.txt' u 1:2 title 'f(x) - dokladne' w lines lt rgb \"#800000\"" << endl;
		ofile << "pause -1" << endl;

	} else {
		fprintf(stderr, "Failed to write to file.\n");
		exit(-1);
	}

	points.close();
	ofile.close();
	org_points.close();

	system("gnuplot lab7.plt");
}

void solve(char method, int steps, double t0,
	double h, double x0, double tn, double xn) {

	double *t = new double[steps];
	double *x = new double[steps];

	t[0] = t0;
	x[0] = x0;

	switch (method) {
		// Euler
		case 'e':
		{
			for (int i = 1; i < steps; i++) {
				double f_val = f(t[i - 1], x[i - 1]);
				x[i] = x[i - 1] + h * f_val;
				t[i] = t[i - 1] + h;
			}
			break;
		}
		// Runge-Kutta
		case 'r':
		{
			for (int i = 1; i < steps; i++) {
				x[i] = x[i - 1] + dy(h, t[i - 1], x[i - 1]);
				t[i] = t[i - 1] + h;
			}
			break;
		}
		// Finite Difference Method
		case 'f':
		{
			t[steps - 1] = tn;
			x[steps - 1] = xn;

			double h = (tn - t0) / (steps - 1);
			
			double A[steps][steps];
			double B[steps];

			int i;

			for (i = 1; i <= steps - 2; i++) {
				t[i] = t[0] + i * h;
				B[i] = pow(h, 2) * ut(t[i]);
			}

			B[0] = pow(h, 2) * ut(t[0]) - (1 + h * wt(t[1]) / 2) * x[0];
			B[steps - 1] = pow(h, 2) * ut(t[steps - 1]) - (1 - h * wt(t[steps - 1]) / 2) * x[steps - 1];

			for (i = 0; i < steps; i++) {
				A[i][i] = - (2 + pow(h, 2) * vt(t[i]));

				
				if (i <= steps - 2) {
					A[i][i + 1] = 1 - (h * wt(t[i])) / 2;
					A[i + 1][i] = 1 + (h * wt(t[i + 1])) / 2;
				}
			}

			// GAUSSIAN
			for (i = 1; i < steps; i++) {
				A[i][i] -= (A[i][i - 1] / A[i - 1][i - 1]) * A[i - 1][i];
				B[i] -= (A[i][i - 1] / A[i - 1][i - 1]) * B[i - 1];
			}

			x[steps - 1] = B[steps - 1] / A[steps - 1][steps - 1];

			for (i = steps - 2; i >= 0; i--) {
				x[i] = (B[i] - A[i][i + 1] * x[i + 1]) / A[i][i];
			}

			break;
		}
	}

	draw(method, steps, t, x);

	delete[] t;
	delete[] x;
}

int main(int argc, char **argv) {

	//solve(argv[1][0], atoi(argv[2]), M_PI / 4, (7 * M_PI / 4) / atoi(argv[2]), f_ex(M_PI / 4), 0.0, 0.0);
	solve(argv[1][0], atoi(argv[2]), 0, 0.01, 0.0, (M_PI + 1) / 2, f_ex((M_PI + 1) / 2));

}