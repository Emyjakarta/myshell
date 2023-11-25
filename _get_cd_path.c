#include "shell.h"

/*#define MAXIMUM_COMMAND_LENGTH 1000*/

char *_get_cd_path(const char *_command, char **arguments) {
	char *_oldpwd = NULL;

	printf("Entered _get_cd_path function\n");

	if (_command == NULL || strcmp(_command, "cd") != 0 || arguments == NULL) {
		printf("Error: Invalid input for _get_cd_path\n");
		return NULL;
	}

	if (arguments[1] == NULL) {
		printf("No arguments provided, returning HOME directory\n");
		return getenv("HOME");
	}
	printf("arguments[1]: %s\n", arguments[1]);
	_oldpwd = getenv("OLDPWD");
	printf("OLDPWD: %s\n", _oldpwd);
	if (_oldpwd == NULL) {
		printf("OLDPWD not set\n");
		return NULL;
	}
	if (strcmp(arguments[1], "-") == 0) {
		return (_oldpwd);
	}
	else {
		/*Check if the directory exists and is accessible*/
		if (access(arguments[1], F_OK) == 0 && access(arguments[1], R_OK | X_OK) == 0) {
			printf("Valid directory: %s\n", arguments[0]);
			return strdup(arguments[1]);
		} else {
			printf("Invalid directory or insufficient permissions\n");
			return NULL;
		}
	}
}