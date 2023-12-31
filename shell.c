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
		read_input(&input);
		printf("input after calling read_input before free(main) : %s\n", input);
		free(input);
		input = NULL;
		printf("input after calling read_input after free(main) : %s\n", input);
	}
	return (0);
}
