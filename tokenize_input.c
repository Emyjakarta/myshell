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
	/*OperatorInfo *operators = NULL;*/
	OperatorInfo operators[MAX_OPERATORS] = {0};
	int op_index = 0;
	/*char *cur_operator = NULL;*/
	/*char *copy_input = NULL;*/
	OperatorInfo current_operator = {0};

	/*copy_input = strdup(input);*/
	printf("input(before getting single_command): %s\n", input);
	single_command = strtok_r(input, delim, &saveptr1);
	printf("input(after getting single_command): %s\n", input);
	printf("Next_command(after getting single_command): %s\n", saveptr1);
	/*command_copy = strdup(single_command);*/
	command_copy = single_command;
	printf("command_copy before NULL check: %s\n", command_copy);
	/*if (command_copy[strlen(command_copy)] == '\0')
	{*/
		/*free(command_copy);
		command_copy = NULL;*/
	/*}*/
	if (command_copy == NULL)
	{
		perror("command_copy is not NULL-terminated\n");
		/*free(operators);
		operators = NULL;*/
		/*free(command_copy);
		command_copy = NULL;*/
		return;
	}
	obtain_operators(&command_copy, operators, MAX_OPERATORS);
	/*operators = obtain_operators(&command_copy);*/
	current_operator = operators[op_index];
	while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
	{
		if (current_operator.operator == NULL || current_operator.operator[0] == '\0')
		{
			execute_command_without_operator(&command_copy, &(*last_exit_status), &current_operator);
			printf("command_copy (if block) for cmd without opera(after executing first command in tokenize_input): %s\n", command_copy);
			/*free(command_copy);
			command_copy = NULL;
			printf("command_copy after free(if block) for cmd without opera(after executing first command in tokenize_input): %s\n", command_copy);*/
		}
		else if (current_operator.operator != NULL) {
			process_commands_with_operators(operators, &(*last_exit_status), &command_copy, &current_operator);
			printf("current_operator.operator after processing commands with operators before free: %s\n", current_operator.operator);
			/*free(operators);
			operators = NULL;*/
			printf("current_operator.operator after processing commands with operators after free: %s\n", current_operator.operator);
			/*printf("command_copy before free(else if block) for cmd with opera(tokenize_input): %s\n", command_copy);*/
			/*free(command_copy);
			  command_copy = NULL;*/
			/*printf("command_copy (else if block) for cmd with opera(tokenize_input): %s\n", command_copy);*/
		}
		/*printf("single_command(before calling strtok_r with NULL): %s\n", single_command);*/
		single_command = strtok_r(NULL, delim, &saveptr1);
		printf("single_command(after calling strtok_r with NULL): %s\n", single_command);
		index++;
		if (single_command == NULL)
		{
			perror("strtok_r");
			/*free(operators);
			operators = NULL;*/
			/*printf("command_copy(before free) when single_command is NULL: %s\n", command_copy);*/
			/*free(command_copy);
			command_copy = NULL;*/
			/*printf("command_copy when single_command is NULL: %s\n", command_copy);*/
		}
		else
		{
			/*if (command_copy != NULL)
		  	{
		  		printf("command_copy before free(else block) before reallocating memory(tokenize_input): %s\n", command_copy);
		  		free(command_copy);
		  		command_copy = NULL;
		  		printf("command_copy after free(else block) before reallocating memory(tokenize_input): %s\n", command_copy);
		  	}*/
		  	command_copy = single_command;
		  	printf("command_copy after reallocating memory(tokenize_input): %s\n", command_copy);
		}
		/*free(command_copy);
		  command_copy = NULL;*/
	}
	printf("current_operator.operator before freeing operators(tokenize_input ending): %s\n", current_operator.operator);
	/*free(current_operator.operator);
	  current_operator.operator = NULL;*/
	/*printf("operators[op_index].operator before free(tokenize_input ending): %s\n", operators[op_index].operator);*/
	/*free(*current_operator);
	current_operator = NULL;*/
	/*free(operators);
	operators = NULL;*/
	/*printf("operators[op_index].operator after free(tokenize_input ending): %s\n", operators[op_index].operator);*/
	printf("command_copy after while loop before free(tokenize_input ending) : %s\n", command_copy);
	/*free(command_copy);
	command_copy = NULL;
	printf("command_copy after while loop(tokenize_input ending) after free: %s\n", command_copy);*/
	/*printf("input after while loop before free(tokenize_input ending) : %s\n", input);*/
	/*if (input != NULL)
	{
		free(input);
		input = NULL;
	}*/
	/*printf("input after while loop after free(tokenize_input ending) : %s\n", input);*/
}
void process_commands_with_operators(OperatorInfo *operators, int *last_exit_status, char **command_copy, OperatorInfo *current_operator) {
	int op_index = 0;
	/*char *original_command_copy = NULL;*/
	char *operator_position = NULL;
	total_operators = count_operators(operators);
	executed_commands_count = 0;

	/*original_command_copy = strdup(*command_copy);*/
	if (*command_copy == NULL || current_operator->operator == NULL)
	{
		free(*command_copy);
		(*command_copy) = NULL;
		/*free(operators);
		operators = NULL;*/
		return;
	}
	else
	{
		while (current_operator->operator != NULL) {
			printf("operators[op_index].operator in process_commands_with_operators: %s\n", operators[op_index].operator);
			printf("current_operator->operator in process_commands_with_operators: %s\n", current_operator->operator);
			operator_position = strstr((*command_copy), current_operator->operator);
			if (operator_position == NULL) {
				free(*command_copy);
				(*command_copy) = NULL;
				/*free(operators);
				operators = NULL;*/
				break;
			}
			else
			{
				process_operator_occurrences(&executed_commands_count, op_index, &total_operators, operators, &(*last_exit_status), *current_operator, (*command_copy), operator_position);
			}
			/*free(operators);
			operators = NULL;*/
			/*printf("original_command_copy(before free) in process_commands_with_operators: %s\n", original_command_copy);*/
			/*free(original_command_copy);
			  original_command_copy = NULL;
			  printf("original_command_copy(after free) before break in process_commands_with_operators: %s\n", original_command_copy);*/
			/*op_index++;*/
			/*cleanup(operators);*/
			/*operator_position = NULL;
			  free(command_copy);
			  command_copy = NULL;*/
			break;
		}
		/*free(operators);
		operators = NULL;*/
		/*free(original_command_copy);
		  original_command_copy = NULL;*/
		/*free(command_copy);
		  command_copy = NULL;*/
	}
	/*printf("original_command_copy(before free) in process_commands_with_operators: %s\n", original_command_copy);*/
	/*free(original_command_copy);
	  original_command_copy = NULL;*/
}
char *duplicate_command(char *command) {
	return strdup(command);
}

char *find_operator_position(char *original_command_copy, char *current_operator) {
	return strstr(original_command_copy, current_operator);
}
void process_operator_occurrences(int *executed_commands_count, int op_index, int *total_operators, OperatorInfo *operators, int *last_exit_status, OperatorInfo current_operator, char *original_command_copy, char *operator_position) {
	/*char *current_command = original_command_copy;*/
	int operator_index = 0;
	/*char *ops_position = NULL;*/
	char *before_operator = NULL;
	char *after_operator = NULL;
	/*char *after_operator_copy = NULL;*/
	(void) op_index;
	(void) operators;

	/*while ((ops_position = strstr(original_command_copy, current_operator.operator)) != NULL) {*/
		operator_index = operator_position - original_command_copy;
		before_operator = extract_before_operator(&original_command_copy, operator_index);
		after_operator = extract_after_operator(&original_command_copy, operator_index, operator_position, current_operator.operator);

		/*after_operator_copy = strdup(after_operator);*/
		printf("Processing before_operator: %s\n", before_operator);
		tokenize_and_process_before_operator(before_operator, &(*last_exit_status), current_operator.operator);
		/*printf("before_operator(after execution, before free): %s\n", before_operator);*/
		free(before_operator);
		before_operator = NULL;
		/*printf("before_operator(after execution, after free): %s\n", before_operator);*/
		printf("Processing after_operator: %s\n", after_operator);
		tokenize_and_process_after_operator(&after_operator, current_operator.operator, &(*last_exit_status), &(*total_operators));
		/*printf("after_operator(after calling tokenize_and_process_after_operator): %s\n", after_operator);*/
		/*free(before_operator);
		  before_operator = NULL;
		  free(after_operator_copy);
		  after_operator_copy = NULL;
		  free(original_command_copy);
		  original_command_copy = NULL;*/
		/*printf("after_operator(after executing last_command(process_operator_occurences): %s\n", after_operator);*/
		/*if (*total_operators != 0) {*/
			/*update_indices_pointers(&operator_index, &op_index, &current_operator, operators, &original_command_copy, &ops_position);*/
			/*operator_position = strstr(original_command_copy, current_operator.operator);*/
		/*}
		else
		{*/
		if ((strcmp(current_operator.operator, "&&") == 0 && *last_exit_status != 0) ||
				(strcmp(current_operator.operator, "||") == 0 && *last_exit_status == 0)) {
				/*free(after_operator_copy);*/
			/*free(after_operator);
			after_operator = NULL;*/
				/*free(original_command_copy);
				original_command_copy = NULL;*/
				/*for (i = 0; i < arg_count_op; i++) {
				  free(command_args_op[i]);
				  command_args_op[i] = NULL;
				  }*/
			if (after_operator != NULL) {
				free(after_operator);
			        after_operator = NULL;
			}
			return;
		}
		printf("EXECUTED_COMMANDS_COUNT: %d\n", *executed_commands_count);
		printf("TOTAL_OPERATORS: %d\n", *total_operators);
		printf("after_operator(last_command): %s\n", after_operator);
		printf("current_operator.operator(last_command): %s\n", current_operator.operator);
		tokenize_and_process_last_command(after_operator, &(*last_exit_status), current_operator.operator);
			/*free(after_operator_copy);*/
			/*after_operator_copy = NULL;*/
			/*printf("after_operator(last_command)(after execution, before free): %s\n", after_operator);
			free(after_operator);
			after_operator = NULL;
			printf("after_operator(last_command)(after execution, after free): %s\n", after_operator);*/
			/*free(before_operator);
			  before_operator = NULL;
			  free(original_command_copy);
			  original_command_copy = NULL;*/
		/*return;*/
		/*}*/
		/*free(before_operator);
		  before_operator = NULL;*/
		/*free(after_operator);
		after_operator = NULL;*/
		 /* free(original_command_copy);
		  original_command_copy = NULL;*/
		/*return;*/
	/*}*/
	printf("Before free\noperator_index: %d\nop_index: %d\ncurrent_operator.operator: %s\noriginal_command_copy: %s\n", operator_index, op_index, current_operator.operator, original_command_copy);
	/*if (after_operator != NULL) {
		free(after_operator);
		after_operator = NULL;
	}
	return;*/
	/*free(before_operator);
	before_operator = NULL;*/
	/*free(after_operator_copy);
	  after_operator_copy = NULL;*/
	/*free(after_operator);
	after_operator = NULL;*/
	/*free(original_command_copy);
	original_command_copy = NULL;*/
	/*printf("After free\noperator_index: %d\nop_index: %d\ncurrent_operator.operator: %s\ncurrent_command: %s\n", operator_index, op_index, current_operator.operator, current_command);*/
	/*return;*/
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
	/*if (*op_index == total_operators - 1)
	  return;
	  else
	  {*/
	(*operator_index)++;
	(*op_index)++;
	(*current_operator) = operators[*operator_index];
	/*}*/
	/*if ((*current_operator).operator == NULL)
	  return;*/
	/*else
	  {
	  if (*ops_position != NULL)*/
	if (*current_command != NULL) {
		free(*current_command);
		*current_command = NULL;
	}
	(*current_command) = (*ops_position) + strlen((*current_operator).operator);
	/*}*/
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
		printf("arg_token_op in the else block: %s\n", arg_token_op);
		command_args_op[arg_count_op] = strdup(arg_token_op);
		/*command_args_op[arg_count_op] = arg_token_op;*/
		printf("Contents of the command_args_op array before calling strtok_r:\n");
		for (i = 0; i < arg_count_op; i++) {
			printf("command_args_op[%d]: %s\n", i, command_args_op[i]);
		}
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

	printf("Execute command before logical operators: %s\n", before_operator);
	for (i = 0; i < arg_count_op; i++) {
		printf("command_args_op[%d]: %s\n", i, command_args_op[i]);
	}
	printf("executed_commands_count before executing first command(before_operator): %d\n", executed_commands_count);
	execute_single_command(command_args_op[0], command_args_op, last_exit_status, current_operator);
	executed_commands_count++;
	printf("executed_commands_count after executing first command(before_operator): %d\n", executed_commands_count);

	for (i = 0; i < arg_count_op; i++) {
		free(command_args_op[i]);
		command_args_op[i] = NULL;
	}
	arg_count_op = 0;
	/*free(before_operator);
	before_operator = NULL;*/
}
void tokenize_and_process_after_operator(char **after_operator, char *opera, int *last_exit_status, int *total_operators) {
	int op_index = 0;
	/*OperatorInfo *operators = NULL;*/
	OperatorInfo operators[MAX_OPERATORS] = {0};
	OperatorInfo current_operator = {0};
	char *operator_position = NULL;
	int sum_operators = 0;
	(void) opera;

	if ((*after_operator)[strlen(*after_operator)] == '\0')
	{
		/*operators = obtain_operators(&after_operator);*/
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
	printf("sum_operators just after count: %d\n", sum_operators);
	if (*after_operator == NULL || current_operator.operator == NULL)
	{
		/*free(operators);
		operators = NULL;*/
		/*free(after_operator);
		after_operator = NULL;*/
		return;
	}
	trim_spaces(*after_operator);
	operator_position = strstr(*after_operator, current_operator.operator);

	printf("after_operator(after trimming): %s\n", *after_operator);
	printf("current_operator.operator(after trimming after_operator): %s\n", current_operator.operator);
	printf("sum_operators(after trimming after_operator): %d\n", sum_operators);
	printf("total_operators(after trimming after_operator): %d\n", *total_operators);
	if (sum_operators != 0)
	{
		if ((strcmp(opera, "&&") == 0 && *last_exit_status != 0) ||
				(strcmp(opera, "||") == 0 && *last_exit_status == 0)) {
			/*free(operators);
			operators = NULL;*/
			free(*after_operator);
			*after_operator = NULL;
			return; /*If logical condition met, exit the loop*/
		}
		process_operator_occurrences(&executed_commands_count, op_index, &(*total_operators), operators, &(*last_exit_status), current_operator, (*after_operator), operator_position);
		free(*after_operator);
		*after_operator = NULL;
	}
	/*printf("after_operator(after calling process_operator_occurences): %s\n", after_operator);
	  printf("after_operator(after executing last_command(after_operator)): %s\n", after_operator);*/
	/*free(operators);
	operators = NULL;*/
	/*free(after_operator);
	  after_operator = NULL;*/
	return;
}
void tokenize_and_process_last_command(char *after_operator, int *last_exit_status, char *current_operator) {

	int arg_count_op = 0;
	char *arg_token_op;
	char *saveptr2;
	int i;
	char *command_args_op[MAX_COMMAND_ARGS];

	trim_spaces(after_operator);
	arg_token_op = strtok_r(after_operator, " \t", &saveptr2);
	while (arg_token_op != NULL && arg_count_op < MAX_COMMAND_ARGS - 1) {
		/*command_args_op[arg_count_op] = arg_token_op;*/
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

	printf("Execute last command: %s\n", after_operator);
	for (i = 0; i < arg_count_op; i++) {
		printf("command_args_op(last_command)[%d]: %s\n", i, command_args_op[i]);
	}
	/*printf("executed_commands_count before executing first command: %d\n", executed_commands_count);*/
	execute_single_command(command_args_op[0], command_args_op, last_exit_status, current_operator);
	/*executed_commands_count++;*/

	for (i = 0; i < arg_count_op; i++) {
		free(command_args_op[i]);
		command_args_op[i] = NULL;
		printf("command_args_op[%d] after free: %s\n", i, command_args_op[i]);
	}
	arg_count_op = 0;
	free(after_operator);
	after_operator = NULL;
	printf("after_operator after executing last_command(tokenize_and_process_last_command): %s\n", after_operator);
	return;
}
void execute_command_without_operator(char **command_copy, int *last_exit_status, OperatorInfo *current_operator)
{
	char /*before_operator,*/ *arg_token, *command_args[MAX_COMMAND_ARGS];
	char *saveptr1 = NULL; /*saveptr2 = NULL,*/ /*arg*/
	int arg_count, i;

	printf("command_copy before tokenizing (for execute_command_without_operator): %s\n", *command_copy);
	/*printf("command(before_operator for execute_command_without_operator): %s\nsaveptr1: %s\n", before_operator, saveptr1);*/
	arg_count = 0;
	arg_token = strtok_r(*command_copy, " \t", &saveptr1);
	printf("arg_token(command)(first token) after tokenizing command_copy(for execute_command_without_operator): %s\n", arg_token);
	printf("Next_token after tokenizing command_copy(for execute_command_without_operator): %s\n", saveptr1);
	/*printf("arg_token: %s\nNext_command(saveptr1): %s\n", arg_token, saveptr1);*/
	while (arg_token != NULL && arg_count < MAX_COMMAND_ARGS - 1)
	{
		/*arg = strdup(arg_token);
		if (arg == NULL)
			perror("strdup");*/
		command_args[arg_count++] = arg_token;
		printf("arg_token(command)(first token) inside while loop before getting next token(for execute_command_without_operator): %s\n", arg_token);
		/*printf("arg(copy of arg_token)(first token) inside while loop before getting next token(for execute_command_without_operator): %s\n", arg);*/
		arg_token = strtok_r(NULL, " \t", &saveptr1);
		printf("arg_token(next_token) inside while loop(for execute_command_without_operator): %s\n", arg_token);
	}
	command_args[arg_count] = NULL;
	printf("contents of the command_args array immediately before calling execute_single_command(for execute_command_without_operator):\n");
	/*printf("command_args[0]main for execute_command_without_operator: %s\n", command_args[0]);*/
	for (i = 0; i < arg_count; i++)
	{
		printf("command_args[%d] for execute_command_without_operator: %s\n", i, command_args[i]);
	}
	execute_single_command(command_args[0], command_args, &(*last_exit_status), current_operator->operator);
	for (i = 0; i < arg_count; i++)
	{
		/*printf("arg before freeing command_args: %s\n", arg);*/
		/*printf("command_args[%d] after executing (before free) for without operator: %s\n", i, command_args[i]);*/
		/*free(command_args[i]);
		command_args[i] = NULL;*/
		printf("command_args[%d] after executing for without operator: %s\n", i, command_args[i]);
	}
	printf("command_copy(before free) after executing(before returning) for without operator: %s\n", *command_copy);
	/*printf("arg after freeing command_args: %s\n", arg);*/
	/*free(*command_copy);
	*command_copy = NULL;*/
	/*printf("command_copy(after free) after executing(before returning) for without operator: %s\n", *command_copy);*/
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
				free(command);
				command = NULL;
				return;
			}
			printf("command after calling build_path: %s\n", command);
		}
		*last_exit_status = execute_command(command, arguments);
		/*free(command);
		command = NULL;*/
		/*for (i = 0; arguments[i] != NULL; i++)
		{
			printf("arguments[%d] after executing (execute_single_command) before free: %s\n", i, arguments[i]);
			free(arguments[i]);
			arguments[i] = NULL;
			printf("arguments[%d] after executing (execute_single_command) after free: %s\n", i, arguments[i]);
		}*/
	}
}
