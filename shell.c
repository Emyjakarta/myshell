#include "shell.h"

int main(int argc, char **argv, char **envp)
{
	char *input = NULL;
	(void)argc;
	(void)argv;
	(void)envp;

	while (1)
	{
		show_prompt();
		read_input(input);
	}
	return (0);
}
