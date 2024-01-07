#include "shell.h"
/**
 * _get_cd_path- get cd path
 * @_command: command
 * @arguments: arguments
 * Return: cd path or NULL
 */
char *_get_cd_path(const char *_command, char **arguments)
{
	static char cd_path[MAX_PATH_LENGTH]; /*Pre-allocated buffer for storing cd path*/
	char *_oldpwd = NULL;

	printf("Entered _get_cd_path function\n");

	if (_command == NULL || strcmp(_command, "cd") != 0 || arguments == NULL)
	{
		printf("Error: Invalid input for _get_cd_path\n");
		return (NULL);
	}

	if (arguments[1] == NULL)
	{
		printf("No arguments provided, returning HOME directory\n");
		return (getenv("HOME"));
	}
	printf("arguments[1]: %s\n", arguments[1]);
	_oldpwd = getenv("OLDPWD");
	printf("OLDPWD: %s\n", _oldpwd);
	if (_oldpwd == NULL)
	{
		printf("OLDPWD not set\n");
		return (NULL);
	}
	if (strcmp(arguments[1], "-") == 0)
		return (_oldpwd);
	/*Check if the directory exists and is accessible*/
	if (access(arguments[1], F_OK) == 0 &&
			access(arguments[1], R_OK | X_OK) == 0)
	{
		printf("Valid directory: %s\n", arguments[1]);
		if (strlen(arguments[1]) < MAX_PATH_LENGTH)
		{
			strcpy(cd_path, arguments[1]);
			return (cd_path);
		}
		else
		{
			printf("Path too long\n");
			return (NULL);
		}
	}
	else
	{
		printf("Invalid directory or insufficient permissions\n");
		return (NULL);
	}
}
