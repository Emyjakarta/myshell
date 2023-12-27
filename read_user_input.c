#include "shell.h"

void read_input(char **input)
{
	ssize_t read = 0;
	size_t buffer = 0;
	char *comment_pos = NULL;
	int last_exit_status = 0;

	if (isatty(STDIN_FILENO))
	{
		read = getline(input, &buffer, stdin);
		if (read == -1)
		{
			perror("Error reading input or EOF");
			/*free(input);*/
			exit(EXIT_FAILURE);
		}
		(*input)[strcspn(*input, "\n")] = '\0';
		comment_pos = strchr(*input, '#');
		if (comment_pos != NULL)
			*comment_pos = '\0';
		remove_quotes(*input);
		tokenize_input(*input, &last_exit_status);
	}
	/*if (input)
	{
		free(input);
		input = NULL;
	}*/
}
