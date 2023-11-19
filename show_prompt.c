#include "shell.h"

void show_prompt(void)
{
	char *c = "EMYJAKARTA$ ";

	write(STDOUT_FILENO, c, strlen(c));
}
