#include "shell.h"

void obtain_operators(char **str, OperatorInfo *operators, int size) {
	int i = 0, count = 0;
	/*OperatorInfo operators[MAX_OPERATORS] = {0};*/
	/*OperatorInfo *temp = NULL;*/

	/*size = MAX_OPERATORS;*/
	if ((*str) == NULL)
		return;

	for (i = 0; (*str)[i] != '\0' && count < size - 1; i++) {
		if (((*str)[i] == '&' && (*str)[i + 1] == '&') || ((*str)[i] == '|' && (*str)[i + 1] == '|')) {
			/*operators = realloc(operators, (count + 1) * sizeof(OperatorInfo));*/
			operators[count].position = i;
			operators[count].operator = (*str)[i] == '&' ? "&&" : "||";
			count++;
		}
	}
	/* Add a null-terminating OperatorInfo at the end of the operators array*/
	/*temp = realloc(operators, (count + 1) * sizeof(OperatorInfo));
	if (temp == NULL)
	{
		free(operators);
		operators = NULL;
		return (NULL);
	}
	else
	{
		operators = temp;
	}*/
	operators[count].position = -1; /*Setting a special position to indicate the end of the array*/
	operators[count].operator = NULL;

	printf("str before free for obtain_operators: %s\n", *str);
	/*free(*str);
	*str = NULL;
	printf("str after free for obtain_operators: %s\n", *str);*/
	/*return operators;*/
}
