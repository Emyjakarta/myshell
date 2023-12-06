#include "shell.h"

int tokenize_input(char *input)
{
	int last_exit_status = 0;
	char *arg_token = NULL, *delim = ";\n", *arg_token_op = NULL, *arg_token_op_aft = NULL;
	int index = 0, i = 0, arg_count = 0, arg_count_op = 0, arg_count_op_aft = 0;
	char *command_args[MAX_COMMAND_ARGS], *command_args_op[MAX_COMMAND_ARGS], *command_args_op_aft[MAX_COMMAND_ARGS];
	char *arg = NULL, *single_command = NULL;
	char *saveptr1 = NULL, *saveptr2 = NULL;
	char *command_copy = NULL;
	char *before_operator = NULL;
	char *after_operator = NULL;
	OperatorInfo *operators = NULL;
	int op_index = 0;
	char *original_command_copy = NULL;
	int success = 0;
	char *after_operator_copy = NULL;
	OperatorInfo current_operator = {0};

	single_command = strtok_r(input, delim, &saveptr1);
	command_copy = strdup(single_command);
	operators = obtain_operators(command_copy);
	current_operator = operators[op_index];
	while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
	{
		if (current_operator.operator == NULL)
		{
			before_operator = command_copy;
			after_operator = NULL;
			printf("Next command: %s\n", saveptr1);
			printf("single command: %s\nsaveptr1: %s\n", before_operator, saveptr1);
			arg_count = 0;
			arg_token = strtok_r(before_operator, " \t", &saveptr2);
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
			last_exit_status = execute_single_command(command_args[0], command_args, last_exit_status, current_operator.operator);
		}
		else
		{
			while (operators[op_index].operator != NULL)
			{
				original_command_copy = strdup(command_copy);
				before_operator = strtok_r(original_command_copy, current_operator.operator, &saveptr1);
				after_operator = strtok_r(NULL, current_operator.operator, &saveptr1);
				printf("before_operator: %s\n", before_operator);
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
					command_args_op[arg_count_op++] = strdup(arg_token_op);
					arg_token_op = strtok_r(NULL, " \t", &saveptr2);
				}
				command_args_op[arg_count_op] = NULL;
				printf("Execute command before logical operators: %s\n", before_operator);
				for (i = 0; i < arg_count_op; i++)
				{
					printf("command_args_op[%d]: %s\n", i, command_args_op[i]);
				}
				success = execute_single_command(before_operator, command_args_op, last_exit_status, current_operator.operator);
				arg_count_op_aft = 0;
				command_args_op_aft[0] = NULL;
				arg_token_op_aft = strtok_r(after_operator, " \t", &saveptr2);
				printf("arg_token_op_aft: %s\nsaveptr2: %s\n", arg_token_op_aft, saveptr2);
				while (arg_token_op_aft != NULL && arg_count_op_aft < MAX_COMMAND_ARGS - 1)
				{
					command_args_op_aft[arg_count_op_aft++] = strdup(arg_token_op_aft);
					arg_token_op_aft = strtok_r(NULL, " \t", &saveptr2);
				}
				command_args_op_aft[arg_count_op_aft] = NULL;
				printf("contents of the command_args_op_aft array after splitting into commands and arguments:\n");
				for (i = 0; i < arg_count_op_aft; i++)
				{
					printf("command_args_op_aft[%d]: %s\n", i, command_args_op_aft[i]);
				}
				if ((strcmp(current_operator.operator, "&&") == 0 && success == 0) ||
						(strcmp(current_operator.operator, "||") == 0 && success != 0)) 
				{
					last_exit_status = execute_single_command(after_operator, command_args_op, last_exit_status, current_operator.operator);
					break;  /*Exit the loop if executed after_operator*/
				}
				after_operator_copy = strdup(after_operator);
				free(original_command_copy);
				original_command_copy = after_operator_copy;/*Update command_copy for the next iteration*/
				op_index++;
			}
			free(operators);
			for (i = 0; i < arg_count; i++) 
			{
				free(command_args[i]);
				command_args[i] = NULL;
			}
			free(command_copy);
			single_command = strtok_r(NULL, delim, &saveptr1);
		}

	}
	return (last_exit_status);
}

int execute_single_command(char *command, char **arguments, int result, char *logical_operator)
{
	int b_result = 0, r_result = 0, e_result = 0;
	printf("Before execution, last_exit_status = %d\n", result);
	printf("Executing Command: %s\n", command);
	printf("Logical operator: %s\n", logical_operator);
	b_result = builtin_handler(command, arguments);
	printf("result of builtin_handler: %d\n", b_result);
	if (b_result == 1) {
		if (command != NULL && command[0] != '/') {
			r_result = relative_path(command, arguments);
			return (r_result);
		} else {
			e_result = execute_command(command, arguments);
			return (e_result);
		}
		printf("After executing commands, result in execute_single_command: %d\n", result);
	}
	else			
		return (b_result);
}
