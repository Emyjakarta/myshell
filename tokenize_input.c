#include "shell.h"

void tokenize_input(char *input, int *last_exit_status)
{
	char *arg_token = NULL, *delim = ";&|";
	int index = 0, i = 0, arg_count = 0;
	char *command_args[MAX_COMMAND_ARGS];
	char *arg = NULL, *single_command = NULL;
	char *saveptr1 = NULL, *saveptr2 = NULL;

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
			/*arg[strlen(arg_token)] = '\0';*/
			command_args[arg_count++] = arg;
			arg_token = strtok_r(NULL, " \t", &saveptr2);
			/*printf("arg_tokenb: %s\nsaveptr2b: %s\n", arg_token, saveptr2);*/
		}
		command_args[arg_count] = NULL;
		if (arg_token != NULL && (strcmp(arg_token, "&&") == 0 || strcmp(arg_token, "||") == 0))
		{
			if (strcmp(arg_token, "&&") == 0)
			{
				if (*last_exit_status == 0)
				{
					execute_single_command(command_args[0], command_args, last_exit_status);
				}
			}
			else if (strcmp(arg_token, "||") == 0)
			{
				if (*last_exit_status != 0)
				{
					execute_single_command(command_args[0], command_args, last_exit_status);
				}
			}
			for (i = 0; i < arg_count; i++) {
				free(command_args[i]);
				command_args[i] = NULL;
			}
			single_command = strtok_r(NULL, delim, &saveptr1);
		}
		else
		{
			execute_single_command(command_args[0], command_args, last_exit_status);
			for (i = 0; i < arg_count; i++) {
				free(command_args[i]);
				command_args[i] = NULL;
			}
			single_command = strtok_r(NULL, delim, &saveptr1);
		}
	}
	for (i = 0; i < arg_count; i++) {
		free(command_args[i]);
		command_args[i] = NULL;
	}
}
void execute_single_command(char *command, char **arguments, int *last_exit_status)
{
	int result = 0;
	/*char *arguments[30] = {NULL};*/

	printf("Executing Command: %s\n", command);
	/*arguments[0] = command;*/
	result = builtin_handler(command, arguments, last_exit_status);
	if (result != 1) {
		if (command[0] != '/') {
			relative_path(command, arguments, last_exit_status);
		} else {
			execute_command(command, arguments, last_exit_status);
		}
	}
	printf("Executing Command: %s\n", command);
	/*for (i = 0; i < arg_count; i++)
	{
		free(command_args[i]);*/
		/*free(command_args[i]);*/
		/*command_args[i] = NULL;
	}*/
	/*printf("single_command: %s\n", single_command);*/
	/*single_command = strtok_r(NULL, delim, &saveptr1);
	printf("single_command: %s\nsaveptr1: %s\n", single_command, saveptr1);*/

}
/*else
{
	printf("Executing Command: %s\n", command_args[0]);
	result = builtin_handler(command_args[0], command_args + 1, last_exit_status);
	if (result != 4) {
		if (command_args[0][0] != '/') {
			relative_path(command_args[0], command_args, last_exit_status);
		} else {
			execute_command(command_args[0], command_args, last_exit_status);
		}
	}
	*printf("Executing Command: %s\n", command_args[0]);
	  builtin_handler(command_args[0], command_args + 1);
	  if (command_args[0][0] != '/')
	  {
	  relative_path(command_args[0], command_args);
	  }
	  else
	  execute_command(command_args[0], command_args);*/
	/*printf("Executing Command: %s\n", command_args[0]);
	for (i = 0; i < arg_count; i++)
	{
		free(command_args[i]);
		free(command_args[i]);*
		command_args[i] = NULL;
	}
	printf("single_command: %s\n", single_command);
	single_command = strtok_r(NULL, delim, &saveptr1);
	printf("single_command: %s\nsaveptr1: %s\n", single_command, saveptr1);
}
}*/
/*for (i = 0; i < arg_count; i++)
{
	_safe_free((void **)&command_args[i]);
	*free(command_args[i]);*/
	/*command_args[i] = NULL;*/
/*}*/
