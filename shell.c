#include "shell.h"

int main(int argc, char **argv, char **envp)
{
	char *input = NULL;
	int last_exit_status = 0;
	char *comment_pos = NULL;
	size_t buffer = 0;
	ssize_t read = 0;
	(void)envp;

	if (argc >= 2)
	{
		last_exit_status = _exe_command_from_file(argv[1]);
		return (last_exit_status);
	}
	while (1)
	{
		show_prompt();
		read = getline(&input, &buffer, stdin);
		if (read == -1)
		{
			if (errno == ENOMEM)
			{
				fprintf(stderr, "malloc failed\n");
				if (input != NULL)
				{
					free(input);
					input = NULL;
				}
				return (exit_handler(NULL, NULL));
			}
			else
			{
				if (input != NULL)
				{
					free(input);
					input = NULL;
				}
				return (exit_handler(NULL, NULL));
			}
		}
		else if (read == 0)
		{
			if (isatty(STDIN_FILENO))
			{
				if (input != NULL)
				{
					free(input);
					input = NULL;
				}
				return (exit_handler(NULL, NULL));
			}
			else
			{
				 if (feof(stdin))
				 {
					if (input != NULL)
					{
						free(input);
						input = NULL;
					}
					return (exit_handler(NULL, NULL));
				 }
			}
		}
		(input)[strcspn(input, "\n")] = '\0';
		comment_pos = strchr(input, '#');
		if (comment_pos != NULL)
			*comment_pos = '\0';
		remove_quotes(input);
		tokenize_input(input, &last_exit_status);
	}
	if (input != NULL)
	{
		free(input);
		input = NULL;
	}
	return (0);
}
