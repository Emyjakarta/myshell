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
	const char *_new_dir = va_arg(ptr, const char *);
	char _current_dir[PATH_MAX], *_oldpwd, *_home_dir;
	const char *ERR_MSG = "OLDPWD not set\n";

	_new_dir = _get_cd_path(_command);
	if (_new_dir == NULL || _new_dir[0] == '\0')
		_new_dir = getenv("HOME");
	_home_dir = getenv("HOME");
	_oldpwd = getenv("OLDPWD");
	if (getcwd(_current_dir, PATH_MAX) == NULL)
	{
		perror("getcwd");
		return;
	}
	if (strcmp(_new_dir, "~") == 0 && _home_dir != NULL)
	{
		_new_dir = _home_dir;
	}
	else if (strcmp(_new_dir, "-") == 0)
	{
		if (_oldpwd == NULL)
		{
			write(STDERR_FILENO, ERR_MSG, _strlen(ERR_MSG));
			_safe_free((void **)&_new_dir);
			return;
		}
		_new_dir = _oldpwd;
	}
	if (chdir(_new_dir) != 0)
	{
		perror("chdir");
		_safe_free((void **)&_new_dir);
		return;
	}
	else
	{
		setenv("OLDPWD", _current_dir, 1);
		setenv("PWD", _new_dir, 1);
	}
	if (_new_dir != NULL && _new_dir != getenv("HOME") && _new_dir != _oldpwd)
		_safe_free((void **)&_new_dir);
	/*if (dir == NULL)
	  {
	  dir = getenv("HOME");
	  return (0);
	  }
	  if (chdir(dir) != 0)
	  {
	  perror("cd");
	  return (1);
	  }
	  return (0);*/
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
