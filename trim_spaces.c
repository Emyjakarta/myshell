#include "shell.h"

void trim_spaces(char *str) 
{
	int start = 0, end = strlen(str) - 1;
	int i = 0;

	/*Trim leading spaces*/
	while (str[start] == ' ' || str[start] == '\t')
		start++;

	/* Trim trailing spaces*/
	while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n'))
		end--;

	/*Shift characters to remove leading spaces*/
	for (i = start; i <= end; i++)
		str[i - start] = str[i];

	/*Null-terminate the trimmed string*/
	str[end - start + 1] = '\0';
}
