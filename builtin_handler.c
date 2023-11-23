#include "shell.h"
bool new_dir_allocated = false;

int builtin_handler(char *command, char **arguments)
{
	int i = 0, result = 0;
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
			result = builtin[i].handler(arguments);
			if (result == -1)
			{
				fprintf(stderr, "Error executing built in command: %s\n", command);
				return (2);
			}
			return (1);
		}
	}
	printf("Command not found: %s\n", command);
	return (-1);
}
int exit_handler(char **arguments)
{
	(void) arguments;

	exit(EXIT_SUCCESS);
}
int cd_handler(char **arguments)
{
	char *_new_dir = _get_cd_path(*arguments);
	/*char *path;*/
	char _current_dir[PATH_MAX], *_oldpwd, *_home_dir;
	const char *ERR_MSG = "OLDPWD not set\n";
	/*(void) arguments;*/
	
	_home_dir = getenv("HOME");
	_oldpwd = getenv("OLDPWD");
	printf("_new_dir before check: %s\n", _new_dir);
	if (_new_dir == NULL || _new_dir[0] == '\0')
		_new_dir = _home_dir;
	 printf("_new_dir after check: %s\n", _new_dir);

	/*path = _get_cd_path(_new_dir);*/
	/*if (path == NULL)
		return (-1);*/
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
	printf("Before chdir: _new_dir: %s, _current_dir: %s, PWD: %s, OLDPWD: %s\n",
			               _new_dir, _current_dir, getenv("PWD"), getenv("OLDPWD"));
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
	printf("After conditionals: _new_dir: %s, _current_dir: %s, PWD: %s, OLDPWD: %s\n",
			       _new_dir, _current_dir, getenv("PWD"), getenv("OLDPWD"));
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
	printf("After chdir: _new_dir: %s, _current_dir: %s, PWD: %s, OLDPWD: %s\n",
			               _new_dir, _current_dir, getenv("PWD"), getenv("OLDPWD"));
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
int pwd_handler(char **arguments)
{
	char cwd[PATH_MAX];
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
