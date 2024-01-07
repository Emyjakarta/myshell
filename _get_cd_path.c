#include "shell.h"
/**
 * _get_cd_path- get cd path
 * @_command: command
 * @arguments: arguments
 * Return: cd path or NULL
 */
char *_get_cd_path(const char *_command, char **arguments)
{
	/*Pre-allocated buffer for storing cd path*/
	static char cd_path[MAX_PATH_LENGTH];
	char *_oldpwd = NULL;

	if (_command == NULL || strcmp(_command, "cd") != 0 || arguments == NULL)
		return (NULL);
	if (arguments[1] == NULL)
		return (getenv("HOME"));
	_oldpwd = getenv("OLDPWD");
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
		return (NULL);
	}
}
