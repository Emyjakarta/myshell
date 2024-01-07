#include "shell.h"
#define ERROR_MSG "Error: Path length exceeds maximum limit\n"
/**
 * build_path-build path for ordinary commands like ls
 * @_command: command
 * Return: The built path to the executable
 */
void build_path(const char *_command, char *_result, size_t result_size)
{
	char *_path = getenv("PATH");
	char _copy_path[PATH_MAX];
	char *dir = NULL;
	char _full_path[PATH_MAX];
	char *_build_path = NULL;
	size_t _dir_len, _command_len;

	if (_path == NULL)
	{
		fprintf(stderr, "Error: Unable to fetch path\n");
		_result[0] = '\0';
		return;
	}
	strncpy(_copy_path, _path, PATH_MAX);
	dir = strtok(_copy_path, ":");
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
			strncpy(_result, _full_path, result_size - 1); /* Copy result to buffer */
			_result[result_size - 1] = '\0'; /* Ensure null termination */
			printf("_build_path: %s\n", _build_path);
			return;
		}
		dir = strtok(NULL, ":");
	}
	_result[0] = '\0'; /* Empty string signifies failure */
}
