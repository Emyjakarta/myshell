#include "shell.h"

void obtain_operators(char **str, OperatorInfo *operators, int size) {
	int i = 0, count = 0;

	if ((*str) == NULL)
		return;

	for (i = 0; (*str)[i] != '\0' && count < size - 1; i++) {
		if (((*str)[i] == '&' && (*str)[i + 1] == '&') || ((*str)[i] == '|' && (*str)[i + 1] == '|')) {
			operators[count].position = i;
			operators[count].operator = (*str)[i] == '&' ? "&&" : "||";
			count++;
		}
	}
	/* Add a null-terminating OperatorInfo at the end of the operators array*/
	operators[count].position = -1; /*Setting a special position to indicate the end of the array*/
	operators[count].operator = NULL;
}
