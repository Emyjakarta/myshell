#include "shell.h"

void read_input(void)
{
	char *input = NULL;
	ssize_t read = 0;
	size_t buffer = 0;
	char *arg_token = NULL;
	char *delim = ";";
	int arg_count = 0;
	char *command_args[MAX_COMMAND_ARGS];
	char *comment_pos = NULL;
	int index = 0, i = 0;
	char *arg = NULL;
	char *single_command = NULL;
	char *saveptr1 = NULL, *saveptr2 = NULL;

	if (isatty(STDIN_FILENO))
	{
		show_prompt();
		read = getline(&input, &buffer, stdin);
		if (read == -1)
		{
			perror("Error reading input or EOF");
			free(input);
			return;
		}
		input[strcspn(input, "\n")] = '\0';
		comment_pos = strchr(input, '#');
		if (comment_pos != NULL)
			*comment_pos = '\0';
		remove_quotes(input);
		single_command = strtok_r(input, delim, &saveptr1);
		while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
		{
			arg_count = 0;
			arg_token = strtok_r(single_command, " \t", &saveptr2);
			while (arg_token != NULL && arg_count < MAX_COMMAND_ARGS - 1)
			{
				arg = strdup(arg_token);
				arg[strlen(arg_token)] = '\0';
				command_args[arg_count++] = arg;
				arg_token = strtok_r(NULL, " \t", &saveptr2);
			}
			command_args[arg_count] = NULL;
			printf("Executing Command: %s\n", command_args[0]);
			execute_command(command_args[0], command_args);
			printf("Executing Command: %s\n", command_args[0]);
			for (i = 0; i < arg_count; i++)
			{
				free(command_args[i]);
				command_args[i] = NULL;
			}
			printf("single_command: %s\n", single_command);
			single_command = strtok_r(NULL, delim, &saveptr1);
			printf("single_command: %s\n", single_command);
		}
	}
	free(input);
}
