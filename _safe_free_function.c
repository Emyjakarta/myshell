#include "shell.h"
/**
 * _safe_free-frees a function
 * @ptr: pointer to a pointer
 * Return: void
 */
void _safe_free(void **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}
