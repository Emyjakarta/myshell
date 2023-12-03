#include "shell.h"
bool new_dir_allocated = false;

int builtin_handler(char *command, char **arguments)
{
	int i = 0, result = 0;
	/*int last_exit_status = 0;*/
	/*if (strcmp(command, "&&") == 0) {
	  if (*last_exit_status == 0) {
	  builtin_handler(command, arguments, last_exit_status);
	  }
	  } else if (strcmp(command, "||") == 0) {
	  if (*last_exit_status != 0) {
	  builtin_handler(command, arguments, last_exit_status);
	  }
	  } else {*/
	BuiltInCommand builtin[] = {
		{"exit", exit_handler},
		{"cd", cd_handler},
		{"pwd", pwd_handler},
		/*	{"setenv", setenv_handler},
			{"unsetenv", unsetenv_handler},*/
		{NULL, NULL}
	};
	(void) command;
	for (i = 0; builtin[i].command != NULL; i++)
	{
		printf("Comparing command: %s with built-in: %s\n", command, builtin[i].command);
		if (command != NULL && strcmp(command, builtin[i].command) == 0)
		{
			printf("Match found, calling handler for: %s\n", builtin[i].command);
			result = builtin[i].handler(command, arguments);
			if (result == -1)
			{
				fprintf(stderr, "Error executing built in command: %s\n", command);
				return (2);
			}
			return (0);
		}
	}
	printf("Command not found: %s\n", command);
	/*}*/
	return (-1);
}
int exit_handler(char *command, char **arguments)
{
	(void) command;
	(void) arguments;

	exit(EXIT_SUCCESS);
}
int cd_handler(char *command, char **arguments) {
	char *_new_dir = _get_cd_path(command, arguments);
	char _current_dir[PATH_MAX], *_oldpwd, *_home_dir;
	struct stat statbuf;
	const char *ERR_MSG = "OLDPWD not set\n";
	/*int new_dir_allocated = 0;*/

	if (_new_dir == NULL) {
		printf("Error: Could not get new directory\n");
		return -1;
	}
	if (getcwd(_current_dir, sizeof(_current_dir)) == NULL) {
		perror("getcwd");
		return -1;
	}
	_home_dir = getenv("HOME");
	_oldpwd = getenv("OLDPWD");
	if (_oldpwd == NULL) {
		write(STDERR_FILENO, ERR_MSG, strlen(ERR_MSG));
		return -1;
	}

	printf("_new_dir before check: %s\n", _new_dir);

	if (_new_dir == NULL)
		_new_dir = _home_dir;
	if (_home_dir == NULL) {
		return -1;
	}
	if (strcmp(_new_dir, "-") == 0) {
		if (_oldpwd == NULL) {
			write(STDERR_FILENO, ERR_MSG, strlen(ERR_MSG));
			return -1;
		}
		_new_dir = _oldpwd;
	}
	printf("After conditionals: _new_dir: %s, _current_dir: %s, PWD: %s, OLDPWD: %s\n",
			_new_dir, _current_dir, getenv("PWD"), getenv("OLDPWD"));
	if (_new_dir == NULL || _new_dir[0] == '\0' || strcmp(_new_dir, "~") == 0) {
		_new_dir = _home_dir;
	}
	printf("_new_dir after check: %s\n", _new_dir);
	printf("Before chdir: _new_dir: %s, _current_dir: %s, PWD: %s, OLDPWD: %s\n",
			_new_dir, _current_dir, getenv("PWD"), getenv("OLDPWD"));
	/*Validate _new_dir*/
	if (stat(_new_dir, &statbuf) == 0) {
		if (S_ISDIR(statbuf.st_mode)) {
			/*_new_dir is a directory*/
			if (chdir(_new_dir) != 0) {
				perror("chdir");
				return (-1);
			} 
		} else {
			/* _new_dir is not a directory*/
			fprintf(stderr, "%s is not a directory.\n", _new_dir);
			return -1;
		}
	}
	else {
		/*stat failed, so _new_dir might not exist*/
		perror("stat");
		return -1;
	}
	printf("After chdir: _new_dir: %s, _current_dir: %s, PWD: %s, OLDPWD: %s\n",
			_new_dir, _current_dir, getenv("PWD"), getenv("OLDPWD"));
	if (getcwd(_current_dir, PATH_MAX) == NULL) {
		perror("getcwd");
		return -1;
	} else {
		if (setenv("OLDPWD", _current_dir, 1) == -1) {
			perror("setenv");
			return -1;
		}
		if (setenv("PWD", _current_dir, 1) == -1) {
			perror("setenv");
			return -1;
		}
	}
	printf("After getcwd: _new_dir: %s, _current_dir: %s, PWD: %s, OLDPWD: %s\n",
			_new_dir, _current_dir, getenv("PWD"), getenv("OLDPWD"));
	return 0;
}
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
