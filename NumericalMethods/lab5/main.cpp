#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

int N;

const int START = - 4;
const int END = 4;

double function_value(double x) {
	return sin(2 * x);
}

double function_1st_der(double x) {
	return 2 * cos(2 * x);
}

double function_2nd_der(double x) {
	return - 4 * sin(2 * x);
}

double *quadratic_spline() {

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

	for (int i = 2; i < N - 1; i++) {
		u[i] = 2 * (h[i - 1] + h[i]) - pow(h[i - 1], 2) / u[i - 1];
		v[i] = b[i] - b[i - 1] - h[i - 1] * v[i - 1] / u[i - 1];
	}

	z[N - 1] = 0.0;

	for (int i = N - 2; i >= 1; i--) {
		z[i] = (v[i] - h[i] * z[i + 1]) / u[i];
	}

	z[0] = 0.0;

	return z;
}

double cubic_value(int i, double x, double *z, double *t, double *y) {
	double result;

	result = (z[i] / (6 * (t[i + 1] - t[i]))) * pow(t[i + 1] - x, 3);
	result += (z[i + 1] / (6 * (t[i + 1] - t[i]))) * pow(x - t[i], 3);
	result += (x - t[i]) * ((y[i + 1] / (t[i + 1] - t[i])) - (z[i + 1] * (t[i + 1] - t[i]) / 6));
	result += ((y[i] / (t[i + 1] - t[i])) - (z[i] * (t[i + 1] - t[i]) / 6)) * (t[i + 1] - x);

	return result;
}

void cubic_spline(double step, double *z, double *t, double *y) {
	double plot_step = (fabs(START) + fabs(END)) / 500;
	double curr = START;

	double fval, splval;

	int i = 0;

	while (curr <= END) {

		i = (int) ((curr - START) / step);

		fval = function_value(curr);
		splval = cubic_value(i, curr, z, t, y);

		cout << curr << " " << fval << " " << splval << endl;

		curr += plot_step;
	}
}

int main(int argc, char **argv) {

	if (argc != 2 || (N = atoi(argv[1])) == 0 ) {
		cout << "usage: ./" << argv[0] << " N" << endl;
		return -1;
	}

	double step = (fabs(START) + fabs(END)) / N;
	double t[N], y[N];

	for (int i = 0; i < N; i++) {
		t[i] = START + i * step;
		y[i] = function_value(t[i]);
	}

	//quadratic_spline();
	double *z = cubic_coefficents(t, y);
	cubic_spline(step, z, t, y);

	return 0;
}