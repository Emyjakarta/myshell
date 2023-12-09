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
	int /*success = 0,*/ total_operators = 0;
	char *operator_position = NULL;
	int operator_index = 0;
	int length_after_operator = 0;
	char *current_command = NULL;
	char *ops_position = NULL;
	OperatorInfo current_operator = {0};

	single_command = strtok_r(input, delim, &saveptr1);
	command_copy = strdup(single_command);
	operators = obtain_operators(command_copy);
	while (operators[op_index].operator != NULL)
	{
		printf("op_index before incrementing op_index and total_operators: %d\n", op_index);
		printf("operators[op_index].operator before incrementing op_index and total_operators: %s\n", operators[op_index].operator);
		total_operators++;
		op_index++;
		printf("op_index after incrementing op_index and total_operators: %d\n", op_index);
		printf("operators[op_index].operator after incrementing op_index and total_operators: %s\n", operators[op_index].operator);
	}
	op_index = 0;
	current_operator = operators[op_index];
	while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
	{
		if (current_operator.operator == NULL || current_operator.operator[0] == '\0')
		{
			before_operator = command_copy;
			/*after_operator = NULL;*/
			printf("Next command: %s\n", saveptr1);
			printf("single command(before_operator): %s\nsaveptr1: %s\n", before_operator, saveptr1);
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
		else if (op_index <= total_operators)
		{
			printf("OP_INDEX B4 RECUR: %d\n", op_index);
			if ((strcmp(current_operator.operator, "&&") == 0 && last_exit_status != 0) ||
					(strcmp(current_operator.operator, "||") == 0 && last_exit_status == 0))
			{
				free(after_operator);
				command_args_op_aft[0] = NULL;
				return (last_exit_status); /*If logical condition met, exit the loop*/
			}
			while (operators[op_index].operator != NULL && operators[op_index].position >= 1)
			{
				if ((strcmp(current_operator.operator, "&&") == 0 && last_exit_status != 0) ||
						(strcmp(current_operator.operator, "||") == 0 && last_exit_status == 0))
				{
					free(after_operator);
					command_args_op_aft[0] = NULL;
					return (last_exit_status); /*If logical condition met, exit the loop*/
				}
				printf("oP_index inside loop: %d\n", op_index);
				original_command_copy = strdup(command_copy);
				printf("original_command_copy: %s\n", original_command_copy);
				operator_position = strstr(original_command_copy, current_operator.operator);
				printf("current_operator.operator: %s\n", current_operator.operator);
				if (operator_position != NULL) {
					current_command = original_command_copy;
					while ((ops_position = strstr(current_command, current_operator.operator)) != NULL)
					{
						if (current_command != NULL && current_operator.operator != NULL) {
							ops_position = strstr(current_command, current_operator.operator);
						}
						printf("Current operator(recursive): %s\n", current_operator.operator);
						/*Calculate the index of the operator in the string*/
						operator_index = ops_position - current_command;
						printf("Operator index: %d\n", operator_index);
						/*Allocate memory for before_operator and after_operator*/
						before_operator = (char *)malloc((operator_index + 1) * sizeof(char));
						if (before_operator == NULL)
							perror("malloc");
						after_operator = (char *)malloc((strlen(current_command) - operator_index - strlen(current_operator.operator) + 1) * sizeof(char));
						if (after_operator == NULL)
							perror("malloc");
						/*Copy part of the string before the operator*/
						strncpy(before_operator, current_command, operator_index);
						before_operator[operator_index] = '\0';
						printf("Before operator: %s\n", before_operator);
						/*Copy part of the string after the operator*/
						length_after_operator = strlen(current_command) - operator_index - strlen(current_operator.operator);
						strncpy(after_operator, operator_position + strlen(current_operator.operator), length_after_operator);
						after_operator[length_after_operator] = '\0';
						printf("After operator: %s\n", after_operator);
						trim_spaces(before_operator);
						trim_spaces(after_operator);
						printf("after_operator(HERE): %s\n", after_operator);
						printf("Next command_op: %s\n", saveptr1);
						printf("single command_op(current_command): %s\nsaveptr1: %s\n", current_command, saveptr1);
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
						last_exit_status = execute_single_command(before_operator, command_args_op, last_exit_status, current_operator.operator);
						arg_count_op_aft = 0;
						command_args_op_aft[0] = NULL;
						arg_token_op_aft = strtok_r(after_operator, " \t", &saveptr2);
						printf("arg_token_op_aft: %s\nsaveptr2: %s\n", arg_token_op_aft, saveptr2);
						while (arg_token_op_aft != NULL && arg_count_op_aft < MAX_COMMAND_ARGS - 1)
						{
							if ((strcmp(current_operator.operator, "&&") == 0 && last_exit_status != 0) ||
									(strcmp(current_operator.operator, "||") == 0 && last_exit_status == 0))
							{
								free(after_operator);
								command_args_op_aft[0] = NULL;
								return (last_exit_status); /*If logical condition met, exit the loop*/
							}
							else if (strcmp(arg_token_op_aft, "&&") == 0 || strcmp(arg_token_op_aft, "||") == 0)
							{
								/*Execute the current command with its arguments*/
								command_args_op_aft[arg_count_op_aft] = NULL; /*Null-terminate the command arguments*/
								printf("contents of the command_args_op_aft(AFT) array after null-terminating upon finding operator:\n");
								for (i = 0; i < arg_count_op_aft; i++)
								{
									printf("command_args_op_aft(AFT) just after finding operator and immediately before execution[%d]: %s\n", i, command_args_op_aft[i]);
								}
								execute_single_command(before_operator, command_args_op_aft, last_exit_status, current_operator.operator);
								command_args_op_aft[0] = NULL;

								/*Start a new command*/
								arg_count_op_aft = 0;
							}
							else
							{
								/*Add the token to the current command's arguments*/
								command_args_op_aft[arg_count_op_aft++] = strdup(arg_token_op_aft);
								printf("contents of the command_args_op_aft(AFT) array after starting a new command:\n");
								for (i = 0; i < arg_count_op_aft; i++)
								{
									printf("command_args_op_aft(AFT) just after starting a new command[%d]: %s\n", i, command_args_op_aft[i]);
								}
							}

							arg_token_op_aft = strtok_r(NULL, " \t", &saveptr2);
							printf("contents of the command_args_op_aft(AFT) array before moving to the next command:\n");
							for (i = 0; i < arg_count_op_aft; i++)
							{
								printf("command_args_op_aft(AFT) just BEFORE moving to the next command [%d]: %s\n", i, command_args_op_aft[i]);
							}
						}

						/*Execute the last command if there is one*/
						if (arg_count_op_aft > 0 && last_exit_status == 0) 
						{
							command_args_op_aft[arg_count_op_aft] = NULL; /*Null-terminate the last command's arguments*/
							printf("contents of the command_args_op_aft(AFT) array for the last command if present:\n");
							for (i = 0; i < arg_count_op_aft; i++)
							{
								printf("command_args_op_aft(AFT) for last COMMAND if present[%d]: %s\n", i, command_args_op_aft[i]);
							}
							execute_single_command(before_operator, command_args_op_aft, last_exit_status, current_operator.operator);
							command_args_op_aft[0] = NULL;
							break;
						}
						else if (arg_count_op_aft > 0 && last_exit_status != 0)
						{
							free(after_operator);
							command_args_op_aft[0] = NULL;
							return (last_exit_status);
						}
						/*if ((strcmp(current_operator.operator, "&&") == 0 && last_exit_status != 0) ||
						  (strcmp(current_operator.operator, "||") == 0 && last_exit_status == 0)) 
						  {
						  free(after_operator);
						  command_args_op_aft[0] = NULL;
						  break;
						  }*/
						else
						{
							tokenize_input(after_operator);
						}
						operator_index++;
						current_operator = operators[operator_index];
						current_command = ops_position + strlen(current_operator.operator);
					}

				}
				free(operators);
			}
		}
		single_command = strtok_r(NULL, delim, &saveptr1);
		index++;
		if (single_command == NULL)
			perror("strtok_r");
		else
			command_copy = strdup(single_command);
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
