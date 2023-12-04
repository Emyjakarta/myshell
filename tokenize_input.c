#include "shell.h"

int tokenize_input(char *input)
{
	int last_exit_status = 0;
	char *arg_token = NULL, *delim = ";\n", *arg_token_op = NULL, *arg_token_op_aft = NULL;
	int index = 0, i = 0, arg_count = 0, arg_count_op = 0, arg_count_op_aft = 0;
	char *command_args[MAX_COMMAND_ARGS], *command_args_op[MAX_COMMAND_ARGS], *command_args_op_aft[MAX_COMMAND_ARGS];
	char *arg = NULL, *single_command = NULL;
	char *saveptr1 = NULL, *saveptr2 = NULL;
	char *logical_operator = NULL;
	char *command_copy = NULL;
	char *before_operator = NULL;
	char *after_operator = NULL;
	size_t operator_index = 0;
	char *operator_location = NULL;

	single_command = strtok_r(input, delim, &saveptr1);
	while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
	{
		command_copy = strdup(single_command);
		if (command_copy == NULL)
			return (50);
		logical_operator = obtain_operator(command_copy);
		if (logical_operator == NULL)
		{
			printf("Next command: %s\n", saveptr1);
			printf("single command: %s\nsaveptr1: %s\n", command_copy, saveptr1);
			arg_count = 0;
			arg_token = strtok_r(command_copy, " \t", &saveptr2);
			printf("arg_token: %s\nsaveptr2: %s\n", arg_token, saveptr2);
			while (arg_token != NULL && arg_count < MAX_COMMAND_ARGS - 1)
			{                                                             
				arg = strdup(arg_token);
				if (arg == NULL)
					perror("strdup");
				command_args[arg_count++] = arg;
				arg_token = strtok_r(NULL, " \t", &saveptr2);
			}
			command_args[arg_count] = NULL;
			printf("contents of the command_args array after inner while loop:\n");
			for (i = 0; i < arg_count; i++)
			{
				printf("command_args[%d]: %s\n", i, command_args[i]);
			}
			last_exit_status = execute_single_command(command_args[0], command_args, last_exit_status, logical_operator);
		}
		else
		{
			operator_location = strstr(command_copy, logical_operator);
		}
		if (logical_operator != NULL)
		{
			before_operator = malloc(strlen(command_copy) + 1);
			if (before_operator == NULL)
				return (19);
			after_operator = malloc(strlen(command_copy) + 1);
			if (after_operator == NULL)
				return (20);
			operator_index = operator_location - command_copy;
			if (operator_index > strlen(command_copy))
				return (52);
			strncpy(before_operator, command_copy, operator_index);
			before_operator[operator_index] = '\0';
			printf("before_operator: %s\n", before_operator);
			if (operator_index + strlen(logical_operator) > strlen(command_copy))
				return (53);
			strncpy(after_operator, command_copy + operator_index + strlen(logical_operator), strlen(command_copy) - operator_index - strlen(logical_operator) + 1);
			after_operator[strlen(command_copy) - operator_index - strlen(logical_operator)] = '\0';
			printf("after_operator: %s\n", after_operator);
			trim_spaces(before_operator);
			trim_spaces(after_operator);

			printf("Next command_op: %s\n", saveptr1);
			printf("single command_op: %s\nsaveptr1: %s\n", command_copy, saveptr1);
			arg_count_op = 0;
			arg_token_op = strtok_r(before_operator, " \t", &saveptr2);
			printf("arg_token_op: %s\nsaveptr2: %s\n", arg_token_op, saveptr2);
			while (arg_token_op != NULL && arg_count_op < MAX_COMMAND_ARGS - 1)
			{
				if (strcmp(arg_token_op, "&&") == 0 || strcmp(arg_token_op, "||") == 0)
				{
					arg_token_op = strtok_r(NULL, " \t", &saveptr2);
					continue;
				}
				command_args_op[arg_count_op++] = strdup(arg_token_op);
				arg_token_op = strtok_r(NULL, " \t", &saveptr2);
			}
			command_args_op[arg_count_op] = NULL;
			arg_count_op_aft = 0;
			command_args_op_aft[0] = NULL;
			arg_token_op_aft = strtok_r(after_operator, " \t", &saveptr2);
			printf("arg_token_op_aft: %s\nsaveptr2: %s\n", arg_token_op_aft, saveptr2);
			while (arg_token_op_aft != NULL && arg_count_op_aft < MAX_COMMAND_ARGS - 1)
			{
				if (strcmp(arg_token_op_aft, "&&") == 0 || strcmp(arg_token_op_aft, "||") == 0)
				{
					arg_token_op_aft = strtok_r(NULL, " \t", &saveptr2);
					continue;
				}
				command_args_op_aft[arg_count_op_aft++] = strdup(arg_token_op_aft);
				arg_token_op_aft = strtok_r(NULL, " \t", &saveptr2);
			}
			command_args_op_aft[arg_count_op_aft] = NULL;
			printf("contents of the command_args_op_aft array after inner while loop:\n");
			for (i = 0; i < arg_count_op_aft; i++)
			{
				printf("command_args_op_aft[%d]: %s\n", i, command_args_op_aft[i]);
			}
			if (strcmp(logical_operator, "&&") == 0)
			{
				printf("Execute command before '&&': %s\n", before_operator);
				last_exit_status = execute_single_command(before_operator, command_args_op, last_exit_status, logical_operator);
				if (last_exit_status == 0)
				{
					printf("Execute command after '&&': %s\n", after_operator);
					last_exit_status = execute_single_command(after_operator, command_args_op_aft, last_exit_status, logical_operator);
				}
			}
			else if (strcmp(logical_operator, "||") == 0)
			{
				printf("Execute command before '||': %s\n", before_operator);
				last_exit_status = execute_single_command(before_operator, command_args_op, last_exit_status, logical_operator);
				if (last_exit_status != 0)
				{
					printf("Execute command after '||': %s\n", after_operator);
					last_exit_status = execute_single_command(after_operator, command_args_op_aft, last_exit_status, logical_operator);
				}
			}
			free(before_operator);
			free(after_operator);
		}
		for (i = 0; i < arg_count; i++) {
			free(command_args[i]);
			command_args[i] = NULL;
		}
		/*free(command_copy);*/
		single_command = strtok_r(NULL, delim, &saveptr1);
		index++;
	}
	return (last_exit_status);
}

int execute_single_command(char *command, char **arguments, int result, char *logical_operator)
{
	printf("Before execution, last_exit_status = %d\n", result);
	printf("Executing Command: %s\n", command);
	printf("Logical operator: %s\n", logical_operator);
	result = builtin_handler(command, arguments);
	printf("result of builtin_handler: %d\n", result);
	if (result == 1) {
		if (command != NULL && command[0] != '/') {
			result = relative_path(command, arguments);
		} else {
			result = execute_command(command, arguments);
		}
		printf("After executing commands, result in execute_single_command: %d\n", result);
	}
	return (result);
}
