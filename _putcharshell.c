#include "shell.h"
/**
 * _putcharshell-print string
 * @str: string to be displayed
 * Return: write the string to the STDOUT_FILENO
 */

char _putcharshell(const char *str)
{
	return (write(STDOUT_FILENO, str, strlen(str)));
}
