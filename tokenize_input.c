#include "shell.h"
int executed_commands_count;
int total_operators;

int count_operators(OperatorInfo *operators) {
	/* Count the total number of operators and return */
	int op_index = 0;
	total_operators = 0;

	while (operators[op_index].operator != NULL) {
		total_operators++;
		op_index++;
	}
	return total_operators;
}
void tokenize_input(char *input, int *last_exit_status)
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
		perror("command_copy is not NULL-terminated\n");
		return;
	}
	obtain_operators(&command_copy, operators, MAX_OPERATORS);
	current_operator = operators[op_index];
	while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
	{
		if (current_operator.operator == NULL || current_operator.operator[0] == '\0')
		{
			execute_command_without_operator(&command_copy, &(*last_exit_status), &current_operator);
		}
		else if (current_operator.operator != NULL) {
			process_commands_with_operators(operators, &(*last_exit_status), &command_copy, &current_operator);
		}
		single_command = strtok_r(NULL, delim, &saveptr1);
		index++;
		if (single_command != NULL)
		{
		  	command_copy = single_command;
		}
	}
}
void process_commands_with_operators(OperatorInfo *operators, int *last_exit_status, char **command_copy, OperatorInfo *current_operator) {
	int op_index = 0;
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
		while (current_operator->operator != NULL) {
			operator_position = strstr((*command_copy), current_operator->operator);
			if (operator_position == NULL) {
				free(*command_copy);
				(*command_copy) = NULL;
				break;
			}
			else
			{
				process_operator_occurrences(&executed_commands_count, op_index, &total_operators, operators, &(*last_exit_status), *current_operator, (*command_copy), operator_position);
			}
			break;
		}
	}
}
char *duplicate_command(char *command) {
	return strdup(command);
}

char *find_operator_position(char *original_command_copy, char *current_operator) {
	return strstr(original_command_copy, current_operator);
}
void process_operator_occurrences(int *executed_commands_count, int op_index, int *total_operators, OperatorInfo *operators, int *last_exit_status, OperatorInfo current_operator, char *original_command_copy, char *operator_position) {
	int operator_index = 0;
	char *before_operator = NULL;
	char *after_operator = NULL;
	(void) executed_commands_count;
	(void) op_index;
	(void) operators;

		operator_index = operator_position - original_command_copy;
		before_operator = extract_before_operator(&original_command_copy, operator_index);
		after_operator = extract_after_operator(&original_command_copy, operator_index, operator_position, current_operator.operator);

		tokenize_and_process_before_operator(before_operator, &(*last_exit_status), current_operator.operator);
		free(before_operator);
		before_operator = NULL;
		tokenize_and_process_after_operator(&after_operator, current_operator.operator, &(*last_exit_status), &(*total_operators));
		if ((strcmp(current_operator.operator, "&&") == 0 && *last_exit_status != 0) ||
				(strcmp(current_operator.operator, "||") == 0 && *last_exit_status == 0)) {
			if (after_operator != NULL) {
				free(after_operator);
			        after_operator = NULL;
			}
			return;
		}
		tokenize_and_process_last_command(after_operator, &(*last_exit_status), current_operator.operator);
}
char *extract_before_operator(char **current_command, int operator_index) {
	char *before_operator = (char *)malloc((operator_index + 1) * sizeof(char));
	if (before_operator == NULL)
		perror("malloc");
	strncpy(before_operator, (*current_command), operator_index);
	before_operator[operator_index] = '\0';
	return before_operator;
}
char *extract_after_operator(char **current_command, int operator_index, char *operator_position, char *current_operator) {
	int length_after_operator = strlen(*current_command) - operator_index - strlen(current_operator);
	char *after_operator = (char *)malloc((length_after_operator + 1) * sizeof(char));
	if (after_operator == NULL)
		perror("malloc");
	strncpy(after_operator, operator_position + strlen(current_operator), length_after_operator);
	after_operator[length_after_operator] = '\0';
	return after_operator;
}
void update_indices_pointers(int *operator_index, int *op_index, OperatorInfo *current_operator, OperatorInfo *operators, char **current_command, char **ops_position) {
	(*operator_index)++;
	(*op_index)++;
	(*current_operator) = operators[*operator_index];
	if (*current_command != NULL) {
		free(*current_command);
		*current_command = NULL;
	}
	(*current_command) = (*ops_position) + strlen((*current_operator).operator);
}
void cleanup(OperatorInfo *operators) {
	free(operators);
	operators = NULL;
}
void tokenize_and_process_before_operator(char *before_operator, int *last_exit_status, char *current_operator) {

	int arg_count_op = 0;
	char *arg_token_op;
	char *saveptr2;
	int i;
	char *command_args_op[MAX_COMMAND_ARGS];

	trim_spaces(before_operator);
	arg_token_op = strtok_r(before_operator, " \t", &saveptr2);
	while (arg_token_op != NULL && arg_count_op < MAX_COMMAND_ARGS - 1) {
		command_args_op[arg_count_op] = strdup(arg_token_op);
		if (command_args_op[arg_count_op] == NULL) {
			perror("strdup");
			for (i = 0; i < arg_count_op; i++) {
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

	execute_single_command(command_args_op[0], command_args_op, last_exit_status, current_operator);
	executed_commands_count++;

	for (i = 0; i < arg_count_op; i++) {
		free(command_args_op[i]);
		command_args_op[i] = NULL;
	}
	arg_count_op = 0;
}
void tokenize_and_process_after_operator(char **after_operator, char *opera, int *last_exit_status, int *total_operators) {
	int op_index = 0;
	OperatorInfo operators[MAX_OPERATORS] = {0};
	OperatorInfo current_operator = {0};
	char *operator_position = NULL;
	int sum_operators = 0;
	(void) opera;

	if ((*after_operator)[strlen(*after_operator)] == '\0')
	{
		obtain_operators(after_operator, operators, MAX_OPERATORS);
	}
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
	{
		return;
	}
	trim_spaces(*after_operator);
	operator_position = strstr(*after_operator, current_operator.operator);

	if (sum_operators != 0)
	{
		if ((strcmp(opera, "&&") == 0 && *last_exit_status != 0) ||
				(strcmp(opera, "||") == 0 && *last_exit_status == 0)) {
			free(*after_operator);
			*after_operator = NULL;
			exit_handler(NULL, NULL); /*If logical condition met, exit the loop*/
		}
		process_operator_occurrences(&executed_commands_count, op_index, &(*total_operators), operators, &(*last_exit_status), current_operator, (*after_operator), operator_position);
		free(*after_operator);
		*after_operator = NULL;
	}
	return;
}
void tokenize_and_process_last_command(char *after_operator, int *last_exit_status, char *current_operator) {

	int arg_count_op = 0;
	char *arg_token_op;
	char *saveptr2;
	int i;
	char *command_args_op[MAX_COMMAND_ARGS];

	if ((strcmp(current_operator, "&&") == 0 && *last_exit_status != 0) ||
			(strcmp(current_operator, "||") == 0 && *last_exit_status == 0)) {
		free(after_operator);
		after_operator = NULL;
		exit_handler(NULL, NULL); /*If logical condition met, exit the loop*/
	}
	trim_spaces(after_operator);
	arg_token_op = strtok_r(after_operator, " \t", &saveptr2);
	while (arg_token_op != NULL && arg_count_op < MAX_COMMAND_ARGS - 1) {
		command_args_op[arg_count_op] = strdup(arg_token_op);
		if (command_args_op[arg_count_op] == NULL) {
			perror("strdup");
			free(after_operator);
			after_operator = NULL;
			exit(EXIT_FAILURE);
		}
		arg_token_op = strtok_r(NULL, " \t", &saveptr2);
		arg_count_op++;
	}
	command_args_op[arg_count_op] = NULL;

	execute_single_command(command_args_op[0], command_args_op, last_exit_status, current_operator);

	for (i = 0; i < arg_count_op; i++) {
		free(command_args_op[i]);
		command_args_op[i] = NULL;
	}
	arg_count_op = 0;
	free(after_operator);
	after_operator = NULL;
	return;
}
void execute_command_without_operator(char **command_copy, int *last_exit_status, OperatorInfo *current_operator)
{
	char *arg_token, *command_args[MAX_COMMAND_ARGS];
	char *saveptr1 = NULL;
	int arg_count;

	arg_count = 0;
	arg_token = strtok_r(*command_copy, " \t", &saveptr1);
	while (arg_token != NULL && arg_count < MAX_COMMAND_ARGS - 1)
	{
		command_args[arg_count++] = arg_token;
		arg_token = strtok_r(NULL, " \t", &saveptr1);
	}
	command_args[arg_count] = NULL;
	execute_single_command(command_args[0], command_args, &(*last_exit_status), current_operator->operator);
}
void execute_single_command(char *command, char **arguments, int *last_exit_status, char *logical_operator)
{
	int b_result = 0;
	char command_buffer[PATH_MAX];
	(void) logical_operator;

	command_buffer[0] = '\0'; /* Initialize buffer */
	b_result = builtin_handler(command, arguments);
	if (b_result == 1) {
		if (command != NULL && *arguments[0] != '/') {
			build_path(command, command_buffer, PATH_MAX);
			if (command_buffer[0] == '\0')
			{
				fprintf(stderr, "%s: not found\n", command);
				*last_exit_status = 5;
				return;
			}
			command = command_buffer; /* Update command to point to the result */
		}
		*last_exit_status = execute_command(&command, arguments);
	}
}
