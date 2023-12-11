#include "shell.h"
#define ERROR_MSG "Error: Path length exceeds maximum limit\n"
/**
 * build_path-build path for ordinary commands like ls
 * @_command: command
 * Return: The built path to the executable
 */
char *build_path(const char *_command)
{
	char *_path = getenv("PATH");
	char *_copy_path = strdup(_path);
	char *dir = strtok(_copy_path, ":");
	char _full_path[PATH_MAX];
	char *_build_path = NULL;
	size_t _dir_len, _command_len;

	while (dir != NULL)
	{
		_dir_len = strlen(dir);
		_command_len = strlen(_command);
		if (_dir_len + 1 + _command_len >= PATH_MAX)
		{
			write(STDERR_FILENO, ERROR_MSG, strlen(ERROR_MSG));
			break;
		}
		strcpy(_full_path, dir);
		_full_path[_dir_len] = '/';
		strcpy(_full_path + _dir_len + 1, _command);
		printf("_full_path before access: %s\n", _full_path);
		if (access(_full_path, X_OK) == 0)
		{
			_build_path = strdup(_full_path);
			printf("_build_path after strdup: %s\n", _build_path);
			break;
		}
		/*else
		{
			fprintf(stderr, "%s: not found\n", _command);*/
			/*printf("arguments[0]: %s\n", arguments[0]);*/
			/*last_exit_status = 5;
			return (last_exit_status);
		}*/
		dir = strtok(NULL, ":");
	}
	free(_copy_path);
	return (_build_path);
}
