#include "shell.h"
/**
 * builtin_handler-builtin handler
 * @command: command
 * @arguments: arguments
 * Return: 0 on success, 1 if not found, 20 if error
 */
int builtin_handler(char *command, char **arguments)
{
	int i = 0, result = 0;
	BuiltInCommand builtin[] = {
		{"exit", exit_handler},
		{"cd", cd_handler},
		{"pwd", pwd_handler},
		/**
		 *	{"setenv", setenv_handler},
		 *	{"unsetenv", unsetenv_handler},
		 */
		{NULL, NULL}
	};
	(void) command;
	for (i = 0; builtin[i].command != NULL; i++)
	{
		if (command != NULL && strcmp(command, builtin[i].command) == 0)
		{
			result = builtin[i].handler(command, arguments);
			if (result == 1)
			{
				fprintf(stderr, "Error executing built in command: %s\n", command);
				return (20);
			}
			return (0);
		}
	}
	return (1);
}
/**
 * exit_handler-exit handler
 * @command: command
 * @arguments: arguments
 * Return: 0
 */
int exit_handler(char *command, char **arguments)
{
	(void) arguments;

	free(command);
	command = NULL;
	exit(EXIT_SUCCESS);
}
/**
 * pwd_handler-pwd handler
 * @command: command
 * @arguments: arguments
 * Return: 0 on success else 1
 */
int pwd_handler(char *command, char **arguments)
{
	char cwd[PATH_MAX];
	(void) command;
	(void) arguments;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
