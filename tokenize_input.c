#include "shell.h"
int executed_commands_count;
int total_operators;

/**
 * tokenize_input-tokenize input
 * @input: input from user
 * @last_exit_status: last exit status. Initialized to 0
 * Return: void
 */
void tokenize_input(char **argv, char *input, int *last_exit_status)
{
	char *delim = ";\n";
	char *single_command = NULL;
	int index = 0;
	char *saveptr1 = NULL;
	char *command_copy = NULL;
	OperatorInfo operators[MAX_OPERATORS] = {0};
	int op_index = 0;
	OperatorInfo current_operator = {0};

	single_command = strtok_r(input, delim, &saveptr1);
	command_copy = single_command;
	if (command_copy == NULL)
	{
		/*perror("command_copy is not NULL-terminated\n");*/
		return;
	}
	obtain_operators(&command_copy, operators, MAX_OPERATORS);
	current_operator = operators[op_index];
	while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
	{
		if (current_operator.operator == NULL ||
				current_operator.operator[0] == '\0')
		{
			execute_command_without_operator(argv[0], &command_copy,
					&(*last_exit_status), &current_operator);
		}
		else if (current_operator.operator != NULL)
		{
			process_commands_with_operators(argv[0], operators, &(*last_exit_status),
					&command_copy, &current_operator);
		}
		single_command = strtok_r(NULL, delim, &saveptr1);
		index++;
		if (single_command != NULL)
		{
			command_copy = single_command;
		}
	}
}
/**
 * process_commands_with_operators-process commands with operators
 * @operators: struct for retrieving logical operators
 * @last_exit_status: last exit status
 * @command_copy: copy of command
 * @current_operator: current operator
 */
void process_commands_with_operators(const char *file_name, OperatorInfo *operators,
		int *last_exit_status, char **command_copy, OperatorInfo *current_operator)
{
	char *operator_position = NULL;

	total_operators = count_operators(operators);
	executed_commands_count = 0;
	if (*command_copy == NULL || current_operator->operator == NULL)
	{
		free(*command_copy);
		(*command_copy) = NULL;
		return;
	}
	else
	{
		while (current_operator->operator != NULL)
		{
			operator_position = strstr((*command_copy), current_operator->operator);
			if (operator_position == NULL)
			{
				free(*command_copy);
				(*command_copy) = NULL;
				break;
			}
			else
			{
				process_operator_occurrences(file_name, &total_operators, &(*last_exit_status),
						*current_operator, (*command_copy), operator_position);
			}
			break;
		}
	}
}
/**
 * process_operator_occurrences-process operator occurrences
 * @total_operators: total operators
 * @last_exit_status: last exit status
 * @current_operator: current operator
 * @original_command_copy: command copy
 * @operator_position: operator position
 */
void process_operator_occurrences(const char *file_name, int *total_operators, int *last_exit_status,
		OperatorInfo current_operator, char *original_command_copy,
		char *operator_position)
{
	int operator_index = 0;
	char *before_operator = NULL;
	char *after_operator = NULL;

	operator_index = operator_position - original_command_copy;
	before_operator = extract_before_operator(&original_command_copy,
			operator_index);
	after_operator = extract_after_operator(&original_command_copy,
			operator_index, operator_position, current_operator.operator);
	tokenize_and_process_before_operator(file_name, before_operator,
			&(*last_exit_status), current_operator.operator);
	free(before_operator);
	before_operator = NULL;
	tokenize_and_process_after_operator(file_name, &after_operator,
			&(*last_exit_status), &(*total_operators));
	if ((strcmp(current_operator.operator, "&&") == 0
				&& *last_exit_status != 0) ||
			(strcmp(current_operator.operator, "||") == 0
			 && *last_exit_status == 0))
	{
		if (after_operator != NULL)
		{
			free(after_operator);
			after_operator = NULL;
		}
		return;
	}
	tokenize_and_process_last_command(file_name, after_operator,
			&(*last_exit_status), current_operator.operator);
}
/**
 * tokenize_and_process_before_operator-tokenize and process before operator
 * @before_operator: command before operator
 * @last_exit_status: last exit status
 * @current_operator: current operator
 */
void tokenize_and_process_before_operator(const char *file_name, char *before_operator,
		int *last_exit_status, char *current_operator)
{

	int arg_count_op = 0;
	char *arg_token_op;
	char *saveptr2;
	int i;
	char *command_args_op[MAX_COMMAND_ARGS];

	trim_spaces(before_operator);
	arg_token_op = strtok_r(before_operator, " \t", &saveptr2);
	while (arg_token_op != NULL && arg_count_op < MAX_COMMAND_ARGS - 1)
	{
		command_args_op[arg_count_op] = strdup(arg_token_op);
		if (command_args_op[arg_count_op] == NULL)
		{
			perror("strdup");
			for (i = 0; i < arg_count_op; i++)
			{
				free(command_args_op[i]);
				command_args_op[i] = NULL;
			}
			free(before_operator);
			before_operator = NULL;
			exit(EXIT_FAILURE);
		}
		arg_token_op = strtok_r(NULL, " \t", &saveptr2);
		arg_count_op++;
	}
	command_args_op[arg_count_op] = NULL;
	execute_single_command(file_name, command_args_op[0], command_args_op,
			last_exit_status, current_operator);
	executed_commands_count++;

	for (i = 0; i < arg_count_op; i++)
	{
		free(command_args_op[i]);
		command_args_op[i] = NULL;
	}
	arg_count_op = 0;
}
/**
 * tokenize_and_process_after_operator-tokenize and process after operator
 * @after_operator: commands after operator
 * @last_exit_status: last exit status
 * @total_operators: total operators
 */
void tokenize_and_process_after_operator(const char *file_name, char **after_operator,
		int *last_exit_status, int *total_operators)
{
	int op_index = 0;
	OperatorInfo operators[MAX_OPERATORS] = {0};
	OperatorInfo current_operator = {0};
	char *operator_position = NULL;
	int sum_operators = 0;

	if ((*after_operator)[strlen(*after_operator)] == '\0')
		obtain_operators(after_operator, operators, MAX_OPERATORS);
	else
	{
		perror("command_copy is not NULL-terminated\n");
		free(*after_operator);
		*after_operator = NULL;
		return;
	}
	current_operator = operators[op_index];
	sum_operators = count_operators(operators);
	if (*after_operator == NULL || current_operator.operator == NULL)
		return;
	trim_spaces(*after_operator);
	operator_position = strstr(*after_operator, current_operator.operator);
	if (sum_operators != 0)
	{
		if ((strcmp(current_operator.operator, "&&") == 0
					&& *last_exit_status != 0) ||
				(strcmp(current_operator.operator, "||") == 0
				 && *last_exit_status == 0))
		{
			free(*after_operator);
			*after_operator = NULL;
			exit_handler(NULL, NULL); /*If logical condition met, exit the loop*/
		}
		process_operator_occurrences(file_name, &(*total_operators),
				&(*last_exit_status), current_operator,
				(*after_operator), operator_position);
		free(*after_operator);
		*after_operator = NULL;
	}
}
