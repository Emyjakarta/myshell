#include "shell.h"

void replace_variables(char *command, char *modified_command, int *last_exit_status) {
	/* Initialize index variables */
	int command_length = strlen(command);
	int modified_index = 0;
	int i = 0, j = 0;
	/*char *loc = NULL;*/
	char *value = NULL;
	char value_holder[MAXIMUM_COMMAND_LENGTH]; /* Buffer to hold environment variable name */
	/*int exit_status = 0;*/

	while (i < command_length - 1 && modified_index < MAXIMUM_COMMAND_LENGTH - 1) {
		if (command[i] == '$') {
			if (command[i + 1] == '?') {
				/* Replace $? */
				/*exit_status = system(command);*/
				if (*last_exit_status == -1) {
					fprintf(stderr, "Command execution failed\n");
					exit(EXIT_FAILURE);
				}
				snprintf(modified_command + modified_index, MAXIMUM_COMMAND_LENGTH - modified_index, "%d", *last_exit_status/*WEXITSTATUS(exit_status)*/);
				modified_index = strlen(modified_command);
				i += 2; /* Skip $? in the command string */
			} else if (command[i + 1] == '$') {
				/* Replace $$ */
				snprintf(modified_command + modified_index, MAXIMUM_COMMAND_LENGTH - modified_index, "%d", getpid());
				modified_index = strlen(modified_command);
				i += 2; /* Skip $$ in the command string */
			} else {
				/* Look for environment variable */
				/*loc = &command[i];*/
				j = i + 1;
				/*while (command[j] != '\0' && command[j] != ' ' && command[j] != '$') {
				  j++;
				  }*/
				while (j < command_length - 1 && command[j] != '\0' && command[j] != ' ' && command[j] != '$') {
					j++;
				}
				strncpy(value_holder, command + i + 1, j - i - 1);
				value_holder[j - i - 1] = '\0';
				/*command[j] = '\0';*/ /* Temporarily terminate to extract the variable name */
				/*value = getenv(loc + 1);*/ /* Get the value of the environment variable */
				value = getenv(value_holder); /* Get the value of the environment variable */
				if (value != NULL) {
					strncat(modified_command, value, MAXIMUM_COMMAND_LENGTH - modified_index - 1);
					modified_index = strlen(modified_command);
				}
				/*command[j] = '$';*/ /* Restore the command string */
				i = j - 1; /* Move index to the character after the environment variable */
			}
		} else {
			/* If neither $? nor $$ */
			modified_command[modified_index++] = command[i++];
		}
	}
	modified_command[modified_index] = '\0'; /* Null-terminate the modified command */
	free(value);
	value = NULL;
}
