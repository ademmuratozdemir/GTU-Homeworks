#include <stdio.h>

#define ROWS 512
#define COLS 512

/* indicates the state of the coordinate */
enum state {
	LAND, /* island soil */
	MARKED /* traversed island soil or water */
};

void mark_island(int i, int j, enum state arr[ROWS][COLS]);

/* reads the input and returns the L value via a pointer */
void read_input(FILE *f, enum state arr[ROWS][COLS])
{
	int i, j;
	int L, n;

	/* read water level */
	fscanf(f, "%d", &L);
	for (i = 0; i < ROWS; ++i) {
		for (j = 0; j < COLS; ++j) {
			fscanf(f, "%d", &n);
			/* is land if above water level */
			if (n > L) {
				arr[i][j] = LAND;
			/* is water otherwise */
			} else {
				arr[i][j] = MARKED;
			}
		}
	}
}

/* scans the array and prints the island coordinates */
void print_output(FILE *f, enum state arr[ROWS][COLS]) {
	int i, j;

	for (i = 0; i < ROWS; ++i) {
		for (j = 0; j < COLS; ++j) {
			/* print the first island soild we can find */
			if (arr[i][j] == LAND) {
				fprintf(f, "%d %d\n", j, i);
				/* mark the island as already discovered */
				mark_island(i, j, arr);
			}
		}
	}
}

int main(void)
{
	FILE *fin, *fout;
	enum state arr[ROWS][COLS];

	fin = fopen("input.txt", "r");
	if (fin == NULL) {
		return 1;
	}

	read_input(fin, arr);
	fclose(fin);

	fout = fopen("output.txt", "w");
	if (fout == NULL) {
		return 1;
	}

	print_output(fout, arr);
	fclose(fout);
	return 0;
}

/* marks the island by traversing the array */
void mark_island(int i, int j, enum state arr[ROWS][COLS])
{
	/* bounds checking */
	if (j < 0 || i < 0 || i >= ROWS || j >= COLS || arr[i][j] == MARKED) {
		return;
	}

	/* mark the current soil block */
	arr[i][j] = MARKED;
	/* mark every soil block left, right, up and down */
	mark_island(i, j - 1, arr);
	mark_island(i, j + 1, arr);
	mark_island(i - 1, j, arr);
	mark_island(i + 1, j, arr);
}
