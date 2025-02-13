#include <stdio.h>
#include <string.h>

#define MAX_POLY_LEN 1000 /* Max length of the polynomial */
#define MAX_NVALUE 100 /* Max number of values */


int read_doubles(FILE *f, double *arr);
void write_evals(FILE *f, double *arr, int len, char *poly);
double power(double val, int n);
double evaluate_exp(char *poly, double val);

int main(void)
{
	FILE *fvals, *fpoly, *fevals;

	char poly[MAX_POLY_LEN + 2]; /* the polynomial with '\n' and '\0' */

	int nvalue; /* number of values */
	double values[MAX_NVALUE]; /* the array of values */

	fvals = fopen("values.txt", "r");
	if (fvals == NULL) {
		printf("can't open values.txt\n");
		return 1;
	}

	nvalue = read_doubles(fvals, values);
	fclose(fvals);

	if (nvalue == -1) {
		printf("error reading values.txt");
		return 1;
	}

	fpoly = fopen("polynomial.txt", "r");
	if (fpoly == NULL) {
		printf("can't open polynomial.txt\n");
		return 1;
	}

	fgets(poly, MAX_POLY_LEN + 2, fpoly);
	fclose(fpoly);

	fevals = fopen("evaluations.txt", "w");
	if (fevals == NULL) {
		printf("can't create evaluations.txt\n");
	}

	write_evals(fevals, values, nvalue, poly);
	fclose(fevals);

	return 0;
}

/* 
 * Reads the file and writes doubles to arr and,
 * returns the number of doubles have been read.
 * Returns -1 if there is an error.
 * WARNING: Assumes the array is large enough.
 */
int read_doubles(FILE *f, double *arr)
{
	int i, n;
	double num;

	i = 0;
	while ((n = fscanf(f, "%lf", &num)) != EOF) {
		if (n != 1)
			return -1;

		arr[i] = num;
		++i;
	}

	return i;
}

/* Writes the result of evaluation poly with values in arr to file. */
void write_evals(FILE *f, double *arr, int len, char *poly)
{
	int i;

	for (i = 0; i < len; ++i) {
		fprintf(f, "%.2f\n", evaluate_exp(poly, arr[i]));
	}
}

/* Returns the nth power of val. */
double power(double val, int n)
{
	double res = 1.0;

	while (n > 0) {
		res *= val;
		--n;
	}

	return res;
}

/* Returns the evaluation of poly with val. */
double evaluate_exp(char *poly, double val)
{
	double res = 0.0;
	double coeff;
	int expo;
	int sign;

	while (*poly != '\0') {
		/* skip whitespace */
		if (*poly == ' ' || *poly == '\n') {
			++poly;
			continue;
		}

		/* determine sign */
		if (*poly == '+') {
			sign = 1;
			++poly;
		} else if (*poly == '-') {
			sign = -1;
			++poly;
		} else {
			sign = 1;
		}

		if (*poly == 'x') {
			coeff = 1.0;
		} else {
			/* read coefficient */
			sscanf(poly, "%lf", &coeff);

			/* skip coefficient */
			while (('0' <= *poly && *poly <= '9') || *poly == '.')
				++poly;
		}

		/* skip x */
		++poly;

		if (*poly == '^') {
			++poly;

			/* read exponent */
			sscanf(poly, "%d", &expo);

			/* skip exponent */
			while ('0' <= *poly && *poly <= '9')
				++poly;
		} else {
			expo = 1.0;
		}

		res += sign * coeff * power(val, expo);
	}

	return res;
}
