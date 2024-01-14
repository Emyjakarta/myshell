#include "shell.h"
#define ERROR_MSG "Error: Path length exceeds maximum limit\n"

int is_command_in_path(const char *command, char **arguments, int *last_exit_status)
{
	char *path = getenv("PATH");
	char copy_path[PATH_MAX];
	char *dir = NULL;
	size_t dir_len = 0;
	size_t command_len = 0;
	char full_path[PATH_MAX];
	char *modified_command = NULL;

	if (path == NULL)
	{
		/* Error fetching path */
		return 0;
	}

	strncpy(copy_path, path, PATH_MAX);

	dir = strtok(copy_path, ":");
	while (dir != NULL)
	{
		dir_len = strlen(dir);
		command_len = strlen(command);
		if (dir_len + 1 + command_len < PATH_MAX)
		{
			strcpy(full_path, dir);
			full_path[dir_len] = '/';
			strcpy(full_path + dir_len + 1, command);

			if (access(full_path, F_OK) == 0)
			{
				printf("full_path during check: %s\n", full_path);
				modified_command = full_path;
				*last_exit_status = execute_command(&modified_command, arguments);
				return (*last_exit_status); /* Command found in PATH */
			}
		}
		dir = strtok(NULL, ":");
	}

	return 0; /* Command not found in PATH */
}
/**
 * build_path-build path for ordinary commands like ls
 * @_command: command
 * @_result: used to build the path
 * @result_size: size of the buffer
 * Return: The built path to the executable
 */
void build_path(const char *_command, char *_result, size_t result_size)
{
	char *_path = getenv("PATH");
	char _copy_path[PATH_MAX];
	char *dir = NULL;
	char _full_path[PATH_MAX];
	size_t _dir_len, _command_len;

	if (_path == NULL)
	{
		/*fprintf(stderr, "Error: Unable to fetch path\n");*/
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
		if (access(_full_path, X_OK) == 0)
		{
			strncpy(_result, _full_path, result_size - 1); /* Copy result to buffer */
			_result[result_size - 1] = '\0'; /* Ensure null termination */
			return;
		}
		dir = strtok(NULL, ":");
	}
	_result[0] = '\0'; /* Empty string signifies failure */
}
