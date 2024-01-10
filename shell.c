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
	/*char modified_input[MAXIMUM_COMMAND_LENGTH] = {'\0'};*/
	int last_exit_status = 0;
	size_t buffer = 0;
	ssize_t read = 0;
	char *modified_input = NULL;
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
		modified_input = handle_variables(input, &last_exit_status);
		/*handle_variables(input, &last_exit_status, input);*/
		/*replace_variables(input, modified_input, &last_exit_status);*/
		tokenize_input(argv, modified_input, &last_exit_status);
		free(modified_input); /* Free memory allocated in handle_variables */
		modified_input = NULL;
	}
	if (input != NULL)
		free(input), input = NULL;
	return (0);
}
