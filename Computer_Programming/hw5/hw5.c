#include <stdio.h>

#define INPUT_LEN 250 /* Max input length */

/* Prints '-' to the file f n times */
void print_indent(FILE *f, int n)
{
	if (n <= 0) {
		return;
	}

	fputc('-', f);
	print_indent(f, n - 1);
}

/* Prints the Newick tree representation of string s to the file f */
void print_tree(FILE *f, char *s, int nparan)
{
	if (*s == '\0') {
		return;
	}

	/* Skip these characters */
	if (*s == ',' || *s == '\t' || *s == ' ' || *s == '\n') {
		print_tree(f, s + 1, nparan);
	/* Increase '-' count by one */
	} else if (*s == '(') {
		print_tree(f, s + 1, nparan + 1);
	/* Decrease '-' count by one */
	} else if (*s == ')') {
		print_tree(f, s + 1, nparan - 1);
	} else {
		print_indent(f, nparan);
		fprintf(f, "%c\n", *s);
		print_tree(f, s + 1, nparan);
	}
}

int main(void)
{
	char input[INPUT_LEN + 1];
	FILE *fin, *fout;

	fin = fopen("input.txt", "r");
	if (fin == NULL) {
		printf("input.txt: can't open file\n");
		return 1;
	}

	fgets(input, INPUT_LEN + 1, fin);
	fclose(fin);

	fout = fopen("output.txt", "w");
	if (fout == NULL) {
		printf("output.txt: can't open file\n");
		return 1;
	}

	print_tree(fout, input, 0);
	fclose(fout);

	return 0;
}
