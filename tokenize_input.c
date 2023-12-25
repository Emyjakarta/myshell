#include "shell.h"

int count_operators(OperatorInfo *operators) {
	/* Count the total number of operators and return */
	int total_operators = 0;
	int op_index = 0;

	while (operators[op_index].operator != NULL) {
		total_operators++;
		op_index++;
	}
	return total_operators;
}
void tokenize_input(char *input, int *last_exit_status)
{
	char *delim = ";\n"; 
	/*arg_token_op = NULL, *arg_token_op_aft = NULL;
	  i = 0, arg_count_op = 0, arg_count_op_aft = 0;
	  char *command_args_op[MAX_COMMAND_ARGS], *command_args_op_aft[MAX_COMMAND_ARGS];*/
	char *single_command = NULL;
	int index = 0; 
	char *saveptr1 = NULL; 
	/*saveptr2 = NULL;*/
	char *command_copy = NULL;
	/*char *before_operator = NULL;
	  char *after_operator = NULL;*/
	OperatorInfo *operators = NULL;
	int op_index = 0;
	/*char *original_command_copy = NULL;
	  char *operator_position = NULL;
	  int operator_index = 0;
	  int length_after_operator = 0;
	  char *current_command = NULL;
	  char *ops_position = NULL;
	  int executed_commands_count = 0;*/
	/*int total_operators = 0;*/
	char *cur_operator = NULL;
	OperatorInfo current_operator = {0};

	single_command = strtok_r(input, delim, &saveptr1);
	command_copy = strdup(single_command);
	operators = obtain_operators(command_copy);
	current_operator = operators[op_index];
	/*total_operators = count_operators(operators);*/
	while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
	{
		if (current_operator.operator == NULL || current_operator.operator[0] == '\0')
		{
			execute_command_without_operator(command_copy, &(*last_exit_status), cur_operator);
		}
		else if (current_operator.operator != NULL) {
			process_commands_with_operators(operators, &(*last_exit_status), &command_copy, &current_operator);
		}
		single_command = strtok_r(NULL, delim, &saveptr1);
		printf("single_command(after calling strtok_r with NULL): %s\n", single_command);
		index++;
		if (single_command == NULL)
			perror("strtok_r");
		else
			command_copy = strdup(single_command);
	}
}
void process_commands_with_operators(OperatorInfo *operators, int *last_exit_status, char **command_copy, OperatorInfo *current_operator) {
	int op_index = 0;
	int executed_commands_count = 0;
	int total_operators = count_operators(operators);
	char *original_command_copy = NULL;
	char *operator_position = NULL;

	while (operators[op_index].operator != NULL) {
		original_command_copy = strdup(*command_copy);
		operator_position = strstr(original_command_copy, current_operator->operator);

		if (operator_position != NULL) {
			process_operator_occurrences(&executed_commands_count, total_operators, operators, &(*last_exit_status), command_copy, *current_operator, original_command_copy, operator_position);
		}
		op_index++;
	}
	cleanup(operators);
}
char *duplicate_command(char *command) {
	return strdup(command);
}

char *find_operator_position(char *original_command_copy, char *current_operator) {
	return strstr(original_command_copy, current_operator);
}
void process_operator_occurrences(int *executed_commands_count, int total_operators, OperatorInfo *operators, int *last_exit_status, char **command_copy, OperatorInfo current_operator, char *original_command_copy, char *operator_position) {
	char *current_command = original_command_copy;
	int operator_index = 0;
	char *ops_position = NULL;
	char *before_operator = NULL;
	char *after_operator = NULL;
	/*char *opera = current_operator.operator;*/
	/*(void) operators;*/
	(void) operator_position;

	while ((ops_position = strstr(current_command, current_operator.operator)) != NULL) {
		operator_index = ops_position - current_command;
		before_operator = extract_before_operator(current_command, operator_index);
		after_operator = extract_after_operator(ops_position, current_operator.operator);

		printf("Processing before_operator: %s\n", before_operator);
		tokenize_and_process_before_operator(before_operator, &(*last_exit_status), current_operator.operator);
		printf("Processing after_operator: %s\n", after_operator);
		tokenize_and_process_after_operator(after_operator, current_operator.operator, &(*last_exit_status));

		update_indices_pointers(&operator_index, &current_operator, operators, &current_command, &ops_position);

		/*free(before_operator);
		  free(after_operator);*/
	}

	if (*executed_commands_count == total_operators + 1) {
		return;
	} else {
		tokenize_and_process_after_operator2(&(*last_exit_status), command_copy, current_operator, after_operator);
	}
	/*operator_index++;
	  op_index++;
	  current_operator = operators[operator_index];
	  current_command = ops_position + strlen(current_operator.operator);
	  printf("New_operator index: %d\n", operator_index);*/
}
char *extract_before_operator(char *current_command, int operator_index) {
	char *before_operator = (char *)malloc((operator_index + 1) * sizeof(char));
	if (before_operator == NULL)
		perror("malloc");
	strncpy(before_operator, current_command, operator_index);
	before_operator[operator_index] = '\0';
	return before_operator;
}
char *extract_after_operator(char *operator_position, char *current_operator) {
	int length_after_operator = strlen(operator_position + strlen(current_operator));
	char *after_operator = (char *)malloc((length_after_operator + 1) * sizeof(char));
	if (after_operator == NULL)
		perror("malloc");
	strncpy(after_operator, operator_position + strlen(current_operator), length_after_operator);
	after_operator[length_after_operator] = '\0';
	return after_operator;
}
void update_indices_pointers(int *operator_index, OperatorInfo *current_operator, OperatorInfo *operators, char **current_command, char **ops_position) {
	(*operator_index)++;
	(*current_operator) = operators[*operator_index];
	(*current_command) = (*ops_position) + strlen((*current_operator).operator);
}
void tokenize_and_process_after_operator2(int *last_exit_status, char **command_copy, OperatorInfo current_operator, char *after_operator) {
	int arg_count_op_aft = 0;
	char *saveptr2;
	char *arg_token_op_aft = NULL;
	char **command_args_op = (char **)malloc(MAX_COMMAND_ARGS * sizeof(char *));
	int i = 0;
	(void) command_copy;

	if (command_args_op == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	arg_token_op_aft = strtok_r(after_operator, " \t", &saveptr2);
	while (arg_token_op_aft != NULL && arg_count_op_aft < MAX_COMMAND_ARGS - 1) {
		command_args_op[arg_count_op_aft] = strdup(arg_token_op_aft);
		if (command_args_op[arg_count_op_aft] == NULL) {
			perror("strdup");
			exit(EXIT_FAILURE);
		}
		arg_count_op_aft++;
		arg_token_op_aft = strtok_r(NULL, " \t", &saveptr2);
	}
	command_args_op[arg_count_op_aft] = NULL;

	/* Execute command using command_args_op */
	execute_single_command(command_args_op[0], command_args_op, last_exit_status, current_operator.operator);

	/* Clean up memory */
	for (i = 0; i < arg_count_op_aft; i++) {
		free(command_args_op[i]);
		command_args_op[i] = NULL;
	}
	free(command_args_op);
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
			exit(EXIT_FAILURE);
		}
		arg_token_op = strtok_r(NULL, " \t", &saveptr2);
		arg_count_op++;
	}
	command_args_op[arg_count_op] = NULL;

	printf("Execute command before logical operators: %s\n", before_operator);
	for (i = 0; i < arg_count_op; i++) {
		printf("command_args_op[%d]: %s\n", i, command_args_op[i]);
	}
	execute_single_command(command_args_op[0], command_args_op, last_exit_status, current_operator);

	for (i = 0; i < arg_count_op; i++) {
		free(command_args_op[i]);
		command_args_op[i] = NULL;
	}
	arg_count_op = 0;
}
void tokenize_and_process_after_operator(char *after_operator, char *opera, int *last_exit_status) {
	char *arg_token_op_aft;
	char *saveptr2;
	char *command_args_op_aft[MAX_COMMAND_ARGS];
	int arg_count_op_aft = 0;
	int i;

	command_args_op_aft[0] = NULL;
	arg_token_op_aft = strtok_r(after_operator, " \t", &saveptr2);

	while (arg_token_op_aft != NULL && arg_count_op_aft < MAX_COMMAND_ARGS - 1) {
		if ((strcmp(opera, "&&") == 0 && *last_exit_status != 0) ||
				(strcmp(opera, "||") == 0 && *last_exit_status == 0)) {
			free(after_operator);
			for (i = 0; i < arg_count_op_aft; i++) {
				free(command_args_op_aft[i]);
				command_args_op_aft[i] = NULL;
			}
			return; /*If logical condition met, exit the loop*/
		} else {
			command_args_op_aft[arg_count_op_aft] = strdup(arg_token_op_aft);
			if (command_args_op_aft[arg_count_op_aft] == NULL) {
				perror("strdup");
				exit(EXIT_FAILURE);
			}
			arg_token_op_aft = strtok_r(NULL, " \t", &saveptr2);
		}
		arg_count_op_aft++;
	}

	command_args_op_aft[arg_count_op_aft] = NULL;

	printf("Contents of the command_args_op_aft array before executing the command:\n");
	for (i = 0; i < arg_count_op_aft; i++) {
		printf("command_args_op_aft[%d]: %s\n", i, command_args_op_aft[i]);
	}

	execute_single_command(command_args_op_aft[0], command_args_op_aft, last_exit_status, opera);
	for (i = 0; i < arg_count_op_aft; i++) {
		free(command_args_op_aft[i]);
		command_args_op_aft[i] = NULL;
	}
}
void execute_command_without_operator(char *command_copy, int *last_exit_status, char *cur_operator)
{
	char *before_operator, *arg_token, *command_args[MAX_COMMAND_ARGS];
	char *saveptr1 = NULL, *saveptr2 = NULL, *arg;
	int arg_count, i;

	before_operator = command_copy;
	printf("Next command(for execute_command_without_operator): %s\n", saveptr1);
	printf("command(before_operator for execute_command_without_operator): %s\nsaveptr1: %s\n", before_operator, saveptr1);
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
	printf("contents of the command_args array immediately before calling execute_single_command(for execute_command_without_operator):\n");
	printf("command_args[0]main for execute_command_without_operator: %s\n", command_args[0]);
	for (i = 0; i < arg_count; i++)
	{
		printf("command_args[%d] for execute_command_without_operator: %s\n", i, command_args[i]);
	}
	execute_single_command(command_args[0], command_args, &(*last_exit_status), cur_operator);
}
void execute_single_command(char *command, char **arguments, int *last_exit_status, char *logical_operator)
{
	int b_result = 0, i = 0;
	printf("Just entering the execute_single_command function\n");
	printf("Before execution, last_exit_status = %d\n", *last_exit_status);
	printf("command after entering execute_single_command: %s\n", command);
	for (i = 0; arguments[i] != NULL; i++)
	{
		printf("arguments[%d] after entering execute_single_command: %s\n", i, arguments[i]);
	}
	printf("Logical operator: %s\n", logical_operator);
	b_result = builtin_handler(command, arguments);
	printf("result of builtin_handler: %d\n", b_result);
	if (b_result == 1) {
		if (command != NULL && *arguments[0] != '/') {
			printf("command before calling build_path: %s\n", command);
			command = build_path(command);
			if (command == NULL)
			{
				fprintf(stderr, "%s: not found\n", command);
				printf("arguments[0]: %s\n", arguments[0]);
				*last_exit_status = 5;
				return;
			}
			printf("command after calling build_path: %s\n", command);
		}
		*last_exit_status = execute_command(command, arguments);
	}
}
