#include "shell.h"
/**
 * main-accepts command line arguments
 * @argc: number of arguments
 * @argv: argument vector
 * @envp: environment variable
 * Return: 0
 */
int main(int argc, char **argv, char **envp)
{
	char *input = NULL, *comment_pos = NULL;
	int last_exit_status = 0;
	size_t buffer = 0;
	ssize_t read = 0;
	(void)envp;

	if (argc >= 2)
	{
		last_exit_status = _exe_command_from_file(argv);
		return (last_exit_status);
	}
	while (1)
	{
		show_prompt();
		read = getline(&input, &buffer, stdin);
		if (read == -1)
		{
			if (input != NULL)
				free(input), input = NULL;
			return (exit_handler(NULL, NULL));
		}
		else if (read == 0)
		{
			if (isatty(STDIN_FILENO))
			{
				if (input != NULL)
					free(input), input = NULL;
				return (exit_handler(NULL, NULL));
			}
		}
		(input)[strcspn(input, "\n")] = '\0';
		comment_pos = strchr(input, '#');
		if (comment_pos != NULL)
			*comment_pos = '\0';
		remove_quotes(input);
		tokenize_input(argv, input, &last_exit_status);
	}
	if (input != NULL)
		free(input), input = NULL;
	return (0);
}
