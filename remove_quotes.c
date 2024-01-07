#include "shell.h"
/**
 * remove_quotes-remove quotes
 * @str: string
 */
void remove_quotes(char *str)
{
	char *src = str;
	char *dst = str;

	while (*src)
	{
		if (*src == '"' || *src == '\'')
		{
			src++;
			continue;
		}
		*dst = *src;
		dst++;
		src++;
	}
	*dst = '\0';
}
