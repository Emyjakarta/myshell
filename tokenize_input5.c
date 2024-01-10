#include "shell.h"

/**
 * handle_variables - handles variables passed to the shell
 * @command: the command to process for variables
 * @exit_code: the exit code of the last process
 * 
 * Return: the updated command with variables expanded if one was found, else
 * the @command is returned as received with no modifications
 */
char *handle_variables(char *command, int *last_exit_status) {
	char *result = malloc(MAXIMUM_COMMAND_LENGTH); /* Allocate memory for the result*/
	size_t command_len = 0;
	size_t result_index = 0;
	int pid = 0;
	int written_chars = 0;
	size_t i = 0, j = 0, value_len = 0;
	char *env_variable = NULL;
	char *env_value = NULL;

	if (result == NULL) {
		/* Handle allocation failure*/
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	result[0] = '\0';
	command_len = strlen(command);
	/*result_index = 0;*/

	for (i = 0; i < command_len; i++) {
		if (command[i] == '$') {
			if (command[i + 1] == '$') { /* PID */
				pid = getpid();
				written_chars = snprintf(result + result_index, MAXIMUM_COMMAND_LENGTH - result_index, "%d", pid);
				if ((size_t)written_chars < 0 || (size_t)written_chars >= MAXIMUM_COMMAND_LENGTH - result_index) {
					fprintf(stderr, "Error writing PID to result\n");
					exit(EXIT_FAILURE);
				}
				result_index += written_chars;
				i++; /* Skip the second '$' */
			} else if (command[i + 1] == '?') { /* Last exit status */
				written_chars = snprintf(result + result_index, MAXIMUM_COMMAND_LENGTH - result_index, "%d", *last_exit_status);
				if ((size_t)written_chars < 0 || (size_t)written_chars >= MAXIMUM_COMMAND_LENGTH - result_index) {
					fprintf(stderr, "Error writing last exit status to result\n");
					exit(EXIT_FAILURE);
				}
				result_index += written_chars;
				i++; /* Skip the '?' */
			} else {
				/* Check for other environment variables*/
				j = i + 1; /* Start index of the potential environment variable*/
				while (command[j] != '\0' && command[j] != ' ') {
					j++;
				}

				env_variable = malloc(j - i);
				if (env_variable == NULL) {
					fprintf(stderr, "Memory allocation failed\n");
					exit(EXIT_FAILURE);
				}
				memset(env_variable, '\0', j - i);
				if (j - i - 1 > 0) {
					strncpy(env_variable, command + i + 1, j - i - 1);
					env_variable[j - i - 1] = '\0';
				} else {
					continue;
				}
				/*strncpy(env_variable, command + i + 1, j - i - 1);
				  env_variable[j - i - 1] = '\0';*/

				printf("before calling getenv:\nenv_variable: %s\nenv_value: %s\n", env_variable, env_value);
				env_value = getenv(env_variable);
				printf("after calling getenv:\nenv_variable: %s\nenv_value: %s\n", env_variable, env_value);
				if (env_value != NULL) {
					value_len = strlen(env_value);
					if (result_index + value_len >= MAXIMUM_COMMAND_LENGTH - 1) {
						fprintf(stderr, "Result exceeds maximum command length\n");
						exit(EXIT_FAILURE);
					}
					strcat(result, env_value);
					result_index += value_len;
				} else {
					strcat(result, "$"); /* Append $ if variable not found*/
					result_index++;
				}
				free(env_variable);
				i = j - 1;
			}
		} else {
			result[result_index++] = command[i];
		}

		if (result_index >= MAXIMUM_COMMAND_LENGTH - 1) {
			fprintf(stderr, "Result exceeds maximum command length\n");
			exit(EXIT_FAILURE);
		}
	}

	result[result_index] = '\0'; /* Null-terminate the result */

	return result; /* Return the modified string */
}
