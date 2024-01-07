#include "shell.h"

int count_operators(OperatorInfo *operators) {
	/* Count the total number of operators and return */
	int op_index = 0;
	int total_operators = 0;

	while (operators[op_index].operator != NULL) {
		total_operators++;
		op_index++;
	}
	return total_operators;
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
void cleanup(OperatorInfo *operators) {
	free(operators);
	operators = NULL;
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
