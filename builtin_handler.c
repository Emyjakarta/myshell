#include "shell.h"

int builtin_handler(char *command, va_list ptr)
{
	int i = 0;
	BuiltInCommand builtin[] = {
		{"exit", exit_handler},
		{"cd", cd_handler},
		{"pwd", pwd_handler},
	/*	{"setenv", setenv_handler},
		{"unsetenv", unsetenv_handler},*/
		{NULL, NULL}
	};
	for (i = 0; builtin[i].command != NULL; i++)
	{
		if (strcmp(command, builtin[i].command) == 0)
		{
			return (builtin[i].handler(ptr));
		}
	}
	return (-1);
}
int exit_handler(void)
{
	exit(EXIT_SUCCESS);
}
int cd_handler(va_list ptr)
{
	const char *dir = va_arg(ptr, const char *);
	if (dir == NULL)
	{
		dir = getenv("HOME");
		return (0);
	}
	if (chdir(dir) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
int pwd_handler(void)
{
	char cwd[PATH_MAX];
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
