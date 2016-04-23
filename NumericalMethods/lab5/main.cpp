#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

int N;

int QUADRATIC_SPLINE = 1;
int CUBIC_SPLINE = 2;

const int START = - 2 * M_PI;
const int END = 2 * M_PI;

double function_value(double x) {
	return 10 + pow(x, 2) / 2 - 10 * cos(2 * x);
}

double function_1st_der(double x) {
	return x + 20 * sin(2 * x);
}

double function_2nd_der(double x) {
	return 40 * cos(2 * x) + 1;
}

double *quadratic_coefficents(double *t, double *y) {
	double *z = new double[N];

	z[0] = function_1st_der(t[0]);
	//z[0] = 150.0;

	for (int i = 0; i < N - 1; i++) {
		z[i + 1] = - z[i] + 2 * ((y[i + 1] - y[i]) / (t[i + 1] - t[i]));
	}

	return z;
}

double *cubic_coefficents(double *t, double *y) {
	double *z = new double[N];

	double h[N], b[N], u[N], v[N];

	for (int i = 0; i < N - 1; i++) {
		h[i] = t[i + 1] - t[i];
		b[i] = 6 * (y[i + 1] - y[i]) / h[i];
	}

	u[1] = 2 * (h[0] + h[1]);
	v[1] = b[1] - b[0];

	for (int i = 2; i < N; i++) {
		u[i] = 2 * (h[i - 1] + h[i]) - pow(h[i - 1], 2) / u[i - 1];
		v[i] = b[i] - b[i - 1] - h[i - 1] * v[i - 1] / u[i - 1];
	}

	// natural
	z[N - 1] = 150.0;
	//z[N - 1] = function_2nd_der(t[N - 1]);


	for (int i = N - 2; i >= 1; i--) {
		z[i] = (v[i] - h[i] * z[i + 1]) / u[i];
	}

	// natural
	z[0] = 150.0;
	//z[0] = function_2nd_der(t[0]);

	return z;
}

double quadratic_value(int i, double x, double *z, double *t, double *y) {
	double result;

	result = ((z[i + 1] - z[i]) / (2 * (t[i + 1] - t[i]))) * pow(x - t[i], 2);
	result += z[i] * (x - t[i]);
	result += y[i];

	return result;
}

double cubic_value(int i, double x, double *z, double *t, double *y) {
	double result;

	result = (z[i] / (6 * (t[i + 1] - t[i]))) * pow(t[i + 1] - x, 3);
	result += (z[i + 1] / (6 * (t[i + 1] - t[i]))) * pow(x - t[i], 3);
	result += (x - t[i]) * ((y[i + 1] / (t[i + 1] - t[i])) - (z[i + 1] * (t[i + 1] - t[i]) / 6));
	result += ((y[i] / (t[i + 1] - t[i])) - (z[i] * (t[i + 1] - t[i]) / 6)) * (t[i + 1] - x);

	return result;
}

void calculate_spline(int type, double step, double *z, double *t, double *y) {
	double plot_step = (fabs(START) + fabs(END)) / 500;
	double curr = START;

	double fval, splval;

	int i = 0;

	double (*fun)(int, double, double*, double*, double*);

	fun = (type == CUBIC_SPLINE) ? cubic_value : quadratic_value;

	double abs_err = 0.0;
	double ls_err = 0.0;

	while (curr <= END) {

		i = (int) ((curr - START) / step);

		fval = function_value(curr);
		splval = fun(i, curr, z, t, y);

		double abs_val = abs(fval - splval);

		if (abs_val > abs_err) abs_err = abs_val;

		ls_err += pow(abs_val, 2);

		cout << curr << " " << fval << " " << splval << endl;

		curr += plot_step;
	}

	cout << "abs_err: " << abs_err << "\tls_err: " << ls_err / N << endl;
}

int main(int argc, char **argv) {

	if (argc != 2 || (N = atoi(argv[1])) == 0 ) {
		cout << "usage: ./" << argv[0] << " N" << endl;
		return -1;
	}

	double step = (fabs(START) + fabs(END)) / (N - 1);
	double t[N], y[N];

	for (int i = 0; i < N; i++) {
		t[i] = START + i * step;
		y[i] = function_value(t[i]);
	}


	double *z = cubic_coefficents(t, y);
	//double *z = quadratic_coefficents(t, y);
	calculate_spline(CUBIC_SPLINE, step, z, t, y);

	return 0;
}