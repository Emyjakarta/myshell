#include "shell.h"

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
