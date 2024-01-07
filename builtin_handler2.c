#include "shell.h"
/**
 * print_directories_info-Prints information about directories.
 * @_new_dir: the new directory path
 * @_current_dir: the current directory path
 */
void print_directories_info(char *_new_dir, char *_current_dir)
{
	printf("After conditionals: _new_dir: %s,\n", _new_dir);
	printf("\t_current_dir: %s, PWD: %s, OLDPWD: %s\n",
			_current_dir, getenv("PWD"), getenv("OLDPWD"));
}

/**
 * change_directory-Changes directory based on the given path.
 * @_new_dir: the new directory path
 * Return: 0 on success, -1 on failure
 */
int change_directory(char *_new_dir)
{
	struct stat statbuf;

	if (stat(_new_dir, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
		{
			if (chdir(_new_dir) != 0)
			{
				perror("chdir");
				return (-1);
			}
		}
		else
		{
			fprintf(stderr, "%s is not a directory.\n", _new_dir);
			return (-1);
		}
	}
	else
	{
		perror("stat");
		return (-1);
	}

	return (0);
}

/**
 * update_environment_variables-Updates environment
 * variables (OLDPWD and PWD) after directory change.
 * @_current_dir: the current directory path
 * Return: 0 on success, -1 on failure
 */
int update_environment_variables(char *_current_dir)
{
	if (setenv("OLDPWD", _current_dir, 1) == -1)
	{
		perror("setenv");
		return (-1);
	}
	if (setenv("PWD", _current_dir, 1) == -1)
	{
		perror("setenv");
		return (-1);
	}
	return (0);
}

/**
 * handle_cd-handle cd
 * @_new_dir: new directory
 * @_current_dir: current directory
 * Return: result, else -1
 */
int handle_cd(char *_new_dir, char *_current_dir)
{
	int result = 0;

	print_directories_info(_new_dir, _current_dir);

	/* Validate and change directory */
	if (change_directory(_new_dir) != 0)
		result = -1;

	printf("After chdir: _new_dir: %s, _current_dir: %s, PWD: %s, OLDPWD: %s\n",
			_new_dir, _current_dir, getenv("PWD"), getenv("OLDPWD"));

	/* Update environment variables */
	if (update_environment_variables(_current_dir) != 0)
		result = -1;

	printf("After getcwd: _new_dir: %s, _current_dir: %s, PWD: %s, OLDPWD: %s\n",
			_new_dir, _current_dir, getenv("PWD"), getenv("OLDPWD"));

	return (result);
}

/**
 * cd_handler-Handles the 'cd' command.
 * @command: command string
 * @arguments: array of arguments
 * Return: 0 on success, -1 on failure
 */
int cd_handler(char *command, char **arguments)
{
	char *_new_dir = _get_cd_path(command, arguments);
	char _current_dir[PATH_MAX], *_oldpwd, *_home_dir;
	const char *ERR_MSG = "OLDPWD not set\n";

	/* Error handling for _new_dir */
	if (_new_dir == NULL)
	{
		printf("Error: Could not get new directory\n");
		return (-1);
	}
	/* Get current directory */
	if (getcwd(_current_dir, sizeof(_current_dir)) == NULL)
	{
		perror("getcwd");
		return (-1);
	}
	/* Get environment variables */
	_home_dir = getenv("HOME");
	_oldpwd = getenv("OLDPWD");
	/* Handling case when OLDPWD is not set */
	if (_oldpwd == NULL)
	{
		write(STDERR_FILENO, ERR_MSG, strlen(ERR_MSG));
		return (-1);
	}
	/* Check and set _new_dir accordingly */
	if (_new_dir == NULL)
		_new_dir = _home_dir;
	if (_home_dir == NULL) /* Check _new_dir and _home_dir */
		return (-1);
	if (strcmp(_new_dir, "-") == 0) /* Handling case when _new_dir is "-" */
	{
		if (_oldpwd == NULL)
		{
			write(STDERR_FILENO, ERR_MSG, strlen(ERR_MSG));
			return (-1);
		}
		_new_dir = _oldpwd;
	}
	return (handle_cd(_new_dir, _current_dir));
}
