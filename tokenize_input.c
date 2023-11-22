#include "shell.h"

void tokenize_input(char *input, ...)
{
	char *arg_token = NULL, *delim = ";";
	int index = 0, i = 0, arg_count = 0;
	char *command_args[MAX_COMMAND_ARGS];
	char *arg = NULL, *single_command = NULL;
	char *saveptr1 = NULL, *saveptr2 = NULL;
	int is_builtin = 0;

	single_command = strtok_r(input, delim, &saveptr1);
	while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
	{
		printf("single command: %s\nsaveptr1: %s\n", single_command, saveptr1);
		arg_count = 0;
		arg_token = strtok_r(single_command, " \t", &saveptr2);
		printf("arg_token: %s\nsaveptr2: %s\n", arg_token, saveptr2);
		while (arg_token != NULL && arg_count < MAX_COMMAND_ARGS - 1)
		{
			arg = strdup(arg_token);
			arg[strlen(arg_token)] = '\0';
			command_args[arg_count++] = arg;
			arg_token = strtok_r(NULL, " \t", &saveptr2);
			printf("arg_tokenb: %s\nsaveptr2b: %s\n", arg_token, saveptr2);
		}
		command_args[arg_count] = NULL;
		printf("Executing Command: %s\n", command_args[0]);
		is_builtin = builtin_handler(command_args[0], NULL);
		if (is_builtin != -1)
		{
			va_list empty_args;
			va_start(empty_args, input);
			builtin_handler(command_args[0], empty_args);
			va_end(empty_args);
		}
		else if (command_args[0][0] != '/')
		{
			relative_path(command_args[0], command_args);
		}
		else
			execute_command(command_args[0], command_args);
		printf("Executing Command: %s\n", command_args[0]);
		for (i = 0; i < arg_count; i++)
		{
			free(command_args[i]);
			command_args[i] = NULL;
		}
		printf("single_command: %s\n", single_command);
		single_command = strtok_r(NULL, delim, &saveptr1);
		printf("single_command: %s\nsaveptr1: %s\n", single_command, saveptr1);
	}
}
