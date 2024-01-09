#include "shell.h"
alias aliases[MAX_ALIASES];
int num_aliases = 0;
/**
 * print_aliases - Prints all defined aliases in the format name='value'.
 * Each alias is printed on a separate line.
 */
void print_aliases() {
	int i = 0;

	for (i = 0; i < num_aliases; ++i) {
		printf("%s='%s'\n", aliases[i].name, aliases[i].value);
	}
}
/**
 * print_alias - Prints the value of a specific alias identified by its name.
 * @name: A pointer to a string containing the name of the alias to print.
 */
void print_alias(char *name) {
	int i = 0;

	for (i = 0; i < num_aliases; ++i) {
		if (strcmp(aliases[i].name, name) == 0) {
			printf("%s='%s'\n", aliases[i].name, aliases[i].value);
			return;
		}
	}
}
/**
 * define_alias - Defines or updates an alias with the given name and value.
 * @name: A pointer to a string containing the name of the alias to define/update.
 * @value: A pointer to a string containing the value to be assigned to the alias.
 */
void define_alias(char *name, char *value) {
	int i = 0;

	for (i = 0; i < num_aliases; ++i) {
		if (strcmp(aliases[i].name, name) == 0) {
			strcpy(aliases[i].value, value);
			return;
		}
	}

	if (num_aliases < MAX_ALIASES) {
		strcpy(aliases[num_aliases].name, name);
		strcpy(aliases[num_aliases].value, value);
		num_aliases++;
	} else {
		fprintf(stderr, "Maximum number of aliases reached\n");
	}
}

/**
 * alias_handler - Handles the 'alias' command to manage aliases
 * @command: command string
 * @arguments: array of arguments
 * Return: 0 on success, -1 on failure
 */
int alias_handler(char *command, char **arguments) {
	char *name = NULL;
	char *value = NULL;
	int i = 0;
	(void) command;

	if (arguments[1] == NULL) {
		print_aliases();
	} else if (arguments[2] == NULL) {
		print_alias(arguments[1]);
	} else {
		i = 1;
		while (arguments[i] != NULL) {
			name = strtok(arguments[i], "=");
			value = strtok(NULL, "=");
			if (value == NULL) {
				fprintf(stderr, "Invalid alias format: %s\n", arguments[i]);
				return -1;
			}
			define_alias(name, value);
			i++;
		}
	}

	return 0;
}

