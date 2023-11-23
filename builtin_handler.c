#include "shell.h"
bool new_dir_allocated = false;

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
		printf("Comparing command: %s with built-in: %s\n", command, builtin[i].command);
		if (strcmp(command, builtin[i].command) == 0)
		{
			printf("Match found, calling handler for: %s\n", builtin[i].command);
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
	char *_new_dir = va_arg(ptr, char *);
	char *path;
	char _current_dir[PATH_MAX], *_oldpwd, *_home_dir;
	const char *ERR_MSG = "OLDPWD not set\n";
	
	_home_dir = getenv("HOME");
	_oldpwd = getenv("OLDPWD");
	if (_new_dir == NULL)
		_new_dir = _home_dir;
	path = _get_cd_path(_new_dir);
	if (path == NULL)
		return (-1);
	if (_home_dir == NULL)
		return (-1);
	if (_new_dir != _home_dir && _new_dir != _oldpwd)
	{
		_new_dir = strdup(_new_dir);
		new_dir_allocated = true;
	}
	
	/*if (_new_dir != NULL)
		printf("_new_dir: %s\n", _new_dir);*/
	if (getcwd(_current_dir, PATH_MAX) == NULL)
	{
		perror("getcwd");
		return (-1);
	}
	if (_new_dir == NULL || strcmp(_new_dir, "~") == 0)
	{
		_new_dir = _home_dir;
	}
	else if (strcmp(_new_dir, "-") == 0)
	{
		if (_oldpwd == NULL)
		{
			write(STDERR_FILENO, ERR_MSG, strlen(ERR_MSG));
			if (new_dir_allocated)
				_safe_free((void **)&_new_dir);
			return (-1);
		}
		_new_dir = _oldpwd;
		new_dir_allocated = false;
	}
	if (chdir(_new_dir) != 0)
	{
		perror("chdir");
		_safe_free((void **)&_new_dir);
		return (-1);
	}
	else
	{
		setenv("OLDPWD", _current_dir, 1);
		setenv("PWD", _new_dir, 1);
	}
	/*if (_new_dir != NULL && _new_dir != _home_dir && _new_dir != _oldpwd)
		_safe_free((void **)&_new_dir);*/
	if (new_dir_allocated)
		_safe_free((void **)&_new_dir);
	_new_dir = NULL;
	new_dir_allocated = false;
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
