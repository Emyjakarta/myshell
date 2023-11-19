#include "shell.h"

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
