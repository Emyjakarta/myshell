#include "shell.h"

int main(int argc, char **argv, char **envp)
{
	char *input = NULL;
	(void)argc;
	(void)argv;
	(void)envp;
	while (1)
	{
		read_input(input);
	}
	return (0);
}
