#include "shell.h"

int main(int argc, char **argv, char **envp)
{
	/*char input[1024];*/
	(void)argc;
	(void)argv;
	(void)envp;
	while (1)
	{
		read_input();
	}
	return (0);
}
