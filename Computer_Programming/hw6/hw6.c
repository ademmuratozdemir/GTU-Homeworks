#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* input instruction INPUT src... */
struct input {
	char **srcs; /* variable number of sources end with NULL */
};

/* and instruction AND dst src1 src2 */
struct and {
	char *dst;
	char *src1;
	char *src2;
};

/* or instruction OR dst src1 src2 */
struct or {
	char *dst;
	char *src1;
	char *src2;
};

/* not instruction NOT dst src */
struct not {
	char *dst;
	char *src;
};

/* flipflop instruction with internal state FLIPFLOP dst src */
struct flipflop {
	char *dst;
	char *src;
	int state;
};

/* instruction types */
enum instruction_type {
	INPUT,
	AND,
	OR,
	NOT,
	FLIPFLOP
};

/* variable instruction value */
union instruction_value {
		struct input input;
		struct and and;
		struct or or;
		struct not not;
		struct flipflop flipflop;
};

/* instruction with link to the next one */
struct instruction {
	enum instruction_type instr; /* determinant */
	union instruction_value val;
	struct instruction *next;
};

/* variable with link to the next one */
/* variables will be used as operands */
struct variable {
	char *name;
	int val;
	struct variable *next;
};

char *get_line(FILE *f);
char **str_split(char *str, char *delim);
void read_instructions(struct instruction **lst, FILE *f);
void free_instructions(struct instruction *lst);
void free_variables(struct variable *lst);
void run_program(FILE *f, struct instruction *instr_lst);

int main(void)
{
	struct instruction *instr_lst = NULL;
	FILE *fcircuit, *finput;

	fcircuit = fopen("circuit.txt", "r");
	if (fcircuit == NULL) {
		return 1;
	}

	read_instructions(&instr_lst, fcircuit);
	fclose(fcircuit);

	finput = fopen("input.txt", "r");
	if (finput == NULL) {
		return 1;
	}

	run_program(finput, instr_lst);
	fclose(finput);

	free_instructions(instr_lst);
	return 0;
}

/* gets current line from file and returns a dynamically allocated string */
char *get_line(FILE *f)
{
	/* growing dynamic array buffer segment size */
	size_t step = 512;
	/* growing dynamic array buffer meaningful length */
	int len = 0;
	char *buf = malloc(step);
	char *newbuf;

	/* failed malloc */
	if (buf == NULL) {
		return NULL;
	}

	for (;;) {
		/* EOF */
		if (fgets(buf + len, step, f) == NULL) {
			free(buf);
			return NULL;
		}

		/* update length */
		len += strlen(buf + len);
		if (buf[len - 1] == '\n') {
			break;
		}

		/* failed realloc */
		newbuf = realloc(buf, len + step);
		if (newbuf == NULL) {
			free(buf);
			return NULL;
		}
		free(buf);
		buf = newbuf;
	}

	/* shrink the string */
	/* failed malloc */
	newbuf = malloc(len + 1);
	if (newbuf == NULL) {
		free(buf);
		return NULL;
	}
	strcpy(newbuf, buf);
	free(buf);

	return newbuf;
}

/* returns a dynamic array containing substrings delimited by delim */
/* the dynamic array will end with NULL */
char **str_split(char *str, char *delim)
{
	char *token;
	char **tks = NULL;
	int len = 0;

	token = strtok(str, delim);
	while (token != NULL) {
		tks = realloc(tks, (len + 1) * sizeof(char *));
		tks[len++] = token;
		token = strtok(NULL, delim);
	}
	tks = realloc(tks, (len + 1) * sizeof(char *));
	/* last element */
	tks[len] = NULL;

	return tks;
}

/* returns a dynamic copy of a string */
char *str_dup(char *str)
{
	int len = strlen(str);
	char *duplicate = malloc(len + 1);
	
	strcpy(duplicate, str);
	return duplicate;
}

/* returns a dynamic copy of a string with specified length */
char *str_ndup(char *str, int len)
{
	char *duplicate = malloc(len + 1);

	strncpy(duplicate, str, len + 1);
	return duplicate;
}

/* returns the length of an array of strings delimited by NULL */
int str_arr_len(char **arr)
{
	int i;

	for (i = 0; arr[i] != NULL; ++i) {
		;
	}

	return i;
}

/* frees an array of strings */
void str_arr_free(char **arr) {
	char **str = arr;

	while (*str != NULL) {
		free(*str);
		++str;
	}
	free(arr);
}

/* creates an input instruction from the operands list */
struct instruction *instr_input_create(char **operands)
{
	int i;
	int len = str_arr_len(operands);
	struct instruction *input = malloc(sizeof(struct instruction));
	char **srcs = malloc((len + 1) * sizeof(char *));

	/* copy operands to the sources list */
	for (i = 0; operands[i] != NULL; ++i) {
		srcs[i] = str_dup(operands[i]);
	}
	srcs[i] = NULL;

	/* assign INPUT determinant */
	input->instr = INPUT;
	/* assign identifiers */
	input->val.input.srcs = srcs;
	input->next = NULL;

	return input;
}

/* creates an and instruction from the operands list */
struct instruction *instr_and_create(char **operands)
{
	struct instruction *and = malloc(sizeof(struct instruction));

	/* assign AND determinant */
	and->instr = AND;
	/* assign identifiers */
	and->val.and.dst = str_dup(operands[0]);
	and->val.and.src1 = str_dup(operands[1]);
	and->val.and.src2 = str_dup(operands[2]);
	and->next = NULL;

	return and;
}

/* creates an or instruction from the operands list */
struct instruction *instr_or_create(char **operands)
{
	struct instruction *or = malloc(sizeof(struct instruction));

	/* assign OR determinant */
	or->instr = OR;
	/* assign identifiers */
	or->val.or.dst = str_dup(operands[0]);
	or->val.or.src1 = str_dup(operands[1]);
	or->val.or.src2 = str_dup(operands[2]);
	or->next = NULL;

	return or;
}

/* creates a not instruction from the operands list */
struct instruction *instr_not_create(char **operands)
{
	struct instruction *not = malloc(sizeof(struct instruction));

	/* assign NOT determinant */
	not->instr = NOT;
	/* assign identifiers */
	not->val.not.dst = str_dup(operands[0]);
	not->val.not.src = str_dup(operands[1]);
	not->next = NULL;

	return not;
}

/* creates a flipflop instruction from the operands list */
struct instruction *instr_flipflop_create(char **operands)
{
	struct instruction *flipflop = malloc(sizeof(struct instruction));

	/* assign FLIPFLOP determinant */
	flipflop->instr = FLIPFLOP;
	/* assign identifiers */
	flipflop->val.flipflop.dst = str_dup(operands[0]);
	flipflop->val.flipflop.src = str_dup(operands[1]);
	/* initial state is 0 */
	flipflop->val.flipflop.state = 0;
	flipflop->next = NULL;

	return flipflop;
}

/* creates an instruction from the string */
struct instruction *instr_create(char *str)
{
	char **tokens = str_split(str, " \n");
	struct instruction *instr = NULL;

	/* take action according to the instruction type */
	if (strcmp(tokens[0], "INPUT") == 0) {
		instr = instr_input_create(&tokens[1]);
	} else if (strcmp(tokens[0], "AND") == 0) {
		instr = instr_and_create(&tokens[1]);
	} else if (strcmp(tokens[0], "OR") == 0) {
		instr = instr_or_create(&tokens[1]);
	} else if (strcmp(tokens[0], "NOT") == 0) {
		instr = instr_not_create(&tokens[1]);
	} else if (strcmp(tokens[0], "FLIPFLOP") == 0) {
		instr = instr_flipflop_create(&tokens[1]);
	}

	free(tokens);
	return instr;
}

/* frees the instructions list */
void free_instructions(struct instruction *lst)
{
	if (lst == NULL) {
		return;
	}

	free_instructions(lst->next);
	
	/* free structs according to the determinant */
	switch (lst->instr) {
	case INPUT:
		str_arr_free(lst->val.input.srcs);
		break;
	case AND:
		free(lst->val.and.dst);
		free(lst->val.and.src1);
		free(lst->val.and.src2);
		break;
	case OR:
		free(lst->val.or.dst);
		free(lst->val.or.src1);
		free(lst->val.or.src2);
		break;
	case NOT:
		free(lst->val.not.dst);
		free(lst->val.not.src);
		break;
	case FLIPFLOP:
		free(lst->val.flipflop.dst);
		free(lst->val.flipflop.src);
		break;
	}
}

/* reads instructions from the file */
void read_instructions(struct instruction **lst, FILE *f)
{
	char *line;
	struct instruction *instr;

	/* append to the linked list until EOF */
	while ((line = get_line(f)) != NULL) {
		instr = instr_create(line);
		*lst = instr;
		lst = &(*lst)->next;
		free(line);
	}
}

/* frees the variables list */
void free_variables(struct variable *lst)
{
	if (lst == NULL) {
		return;
	}

	free_variables(lst->next);

	free(lst->name);
	free(lst);
}

/* finds a variable by name in the variables list */
struct variable *find_variable(struct variable *lst, char *name)
{
	if (lst == NULL) {
		return NULL;
	}

	if (strcmp(lst->name, name) == 0) {
		return lst;
	}

	return find_variable(lst->next, name);
}

/* finds the end of the variables list */
struct variable **find_variables_end(struct variable **lst)
{
	if (lst == NULL) {
		return NULL;
	}

	if (*lst == NULL) {
		return lst;
	}

	return find_variables_end(&(*lst)->next);
}

/* creates a new variable */
struct variable *var_create(char *name, int val, struct variable *next)
{
	struct variable *var = malloc(sizeof(struct variable));

	var->name = name;
	var->val = val;
	var->next = next;

	return var;
}

/* evaluates input instruction */
void instr_eval_input(char *line,
		struct instruction *instr,
		struct variable **lst)
{
	char **operands = str_split(line, " \n");
	char **p;
	struct variable *var = NULL;
	char **srcs = instr->val.input.srcs;

	/* create a variables linked list with values from input
	 * and source indentifiers from the INPUT instruction */
	for (p = operands; *p != NULL; ++p) {
		var = var_create(str_dup(*srcs), atoi(*p), var);
		++srcs;
	}

	*find_variables_end(lst) = var;
	free(operands);
}

/* evaluates and instruction */
int instr_eval_and(struct instruction *instr,
		struct variable **lst)
{
	int dst, src1, src2;

	/* find source values and create destination value */
	src1 = find_variable(*lst, instr->val.and.src1)->val;
	src2 = find_variable(*lst, instr->val.and.src2)->val;
	dst = src1 && src2;

	/* append new variable to the variables list */
	*find_variables_end(lst) = var_create(
			str_dup(instr->val.and.dst), dst, NULL);

	return dst;
}

/* evaluates or instruction */
int instr_eval_or(struct instruction *instr,
		struct variable **lst)
{
	int dst, src1, src2;

	/* find source values and create destination value */
	src1 = find_variable(*lst, instr->val.or.src1)->val;
	src2 = find_variable(*lst, instr->val.or.src2)->val;
	dst = src1 || src2;

	/* append new variable to the variables list */
	*find_variables_end(lst) = var_create(
			str_dup(instr->val.or.dst), dst, NULL);

	return dst;
}

/* evaluates not instruction */
int instr_eval_not(struct instruction *instr,
		struct variable **lst)
{
	int dst, src;

	/* find source value and create destination value */
	src = find_variable(*lst, instr->val.not.src)->val;
	dst = !src;

	/* append new variable to the variables list */
	*find_variables_end(lst) = var_create(
			str_dup(instr->val.not.dst), dst, NULL);

	return dst;
}

/* evaluates flipflop instruction */
int instr_eval_flipflop(struct instruction *instr,
		struct variable **lst)
{
	int dst, src, old_state;

	/* find source value and get old state */
	src = find_variable(*lst, instr->val.flipflop.src)->val;
	old_state = instr->val.flipflop.state;

	/* create destination value according to xor rules */
	dst = (src || old_state) && !(src && old_state);

	/* new state */
	instr->val.flipflop.state = dst;

	/* append new variable to the variables list */
	*find_variables_end(lst) = var_create(
			str_dup(instr->val.flipflop.dst), dst, NULL);

	return dst;
}

/* evaluates instruction and returns the result adds new variables to list*/
int evaluate_instruction(struct instruction *instr,
		struct variable **lst)
{
	/* evaluate instruction according to the determinant */
	switch (instr->instr) {
	case AND:
		return instr_eval_and(instr, lst);
	case OR:
		return instr_eval_or(instr, lst);
	case NOT:
		return instr_eval_not(instr, lst);
	case FLIPFLOP:
		return instr_eval_flipflop(instr, lst);
	case INPUT:
	default:
		return 0;
	}
}

/* reads variables and prints results */
void run_program(FILE *f, struct instruction *instr_lst)
{
	struct variable *var_lst = NULL;
	struct instruction *cur;
	char *line;
	int result = 0;

	while ((line = get_line(f)) != NULL) {
		/* initial case for reading input */
		cur = instr_lst;
		instr_eval_input(line, cur, &var_lst);
		cur = cur->next;
		free(line);
		/* done creating current line's input */

		/* create results */
		while (cur != NULL) {
			result = evaluate_instruction(cur, &var_lst);
			cur = cur->next;
		}
		printf("%d\n", result);

		/* clean the variables list for next line */
		free_variables(var_lst);
		var_lst = NULL;
	}
}
