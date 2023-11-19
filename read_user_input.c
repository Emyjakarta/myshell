#include "shell.h"

void read_input(char *input)
{
	ssize_t read = 0;
	size_t buffer = 0;
	char *arg_token = NULL;
	char *delim = ";", *delim2 = " \t";
	int i = 0, arg_count = 0;
	char *command_args[MAX_COMMAND_ARGS];
	char *comment_pos = NULL, *command = NULL;

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
		command = strtok(input, delim);
		while (command != NULL)
		{
			printf("Command: %s\n", command);
			arg_token = strtok(command, delim2);
			while (arg_token != NULL)
			{
				command_args[arg_count++] = strdup(arg_token);
				printf("Argument/flag: %s\n", arg_token);
				arg_token = strtok(NULL, delim2);
			}
			command_args[arg_count] = NULL;
			execute_command(command_args);
			for (i = 0; i < arg_count; ++i)
			{
				printf("%s ", command_args[i]);
			/*	free(command_args[i]);*/
			}
			command = strtok(NULL, delim);
		}
	}
	free(input);
}
