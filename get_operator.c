#include "shell.h"

OperatorInfo *obtain_operators(char *str) {
	int i = 0, count = 0;
	OperatorInfo *operators = NULL;

	if (str == NULL)
		return NULL;

	count = 0;

	for (i = 0; str[i] != '\0'; i++) {
		if ((str[i] == '&' && str[i + 1] == '&') || (str[i] == '|' && str[i + 1] == '|')) {
			operators = realloc(operators, (count + 1) * sizeof(OperatorInfo));
			operators[count].position = i;
			operators[count].operator = str[i] == '&' ? "&&" : "||";
			count++;
		}
	}
	/* Add a null-terminating OperatorInfo at the end of the operators array*/
	operators = realloc(operators, (count + 1) * sizeof(OperatorInfo));
	operators[count].position = -1; /*Setting a special position to indicate the end of the array*/
	operators[count].operator = NULL;


	return operators;
}

