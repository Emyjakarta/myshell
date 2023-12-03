#include "shell.h"
/**
 * obtain_operator - return && or ||  in a string if found
 * @str: the string to be searched
 * Return: the logical operator if found, else NULL
 */
char *obtain_operator(char *str)
{
	char *operator = NULL;
	int i;

	if (str == NULL)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '&' && str[i + 1] == '&')
		{
			operator = "&&";
			break;
		}
		else if (str[i] == '|' && str[i + 1] == '|')
		{
			operator = "||";
			break;
		}
	}
	return (operator);
}
