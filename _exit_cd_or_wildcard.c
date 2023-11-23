#include "shell.h"
/**
 * _is_exit-check if it is exit command
 * @_command: command to be checked
 * Return: 1 if the command is exit or EOF
 */
int _is_exit(const char *_command)
{
	if (strncmp((char *)_command, "exit", 4) == 0 ||
			strcmp(_command, "EOF") == 0)
	{
		return (1);
	}
	return (0);
}
/**
 * _is_cd-check if it is cd command
 * @_command: command to be checked
 * Return: 1 if the command is cd, 0 otherwise
 */
int _is_cd(const char *_command)
{
	return (strncmp(_command, "cd", 2) == 0);
}
/**
 * _get_cd_path-locate cd path once it is cd
 * @_command: command to be checked
 * Return: pointer to the string containing path following cd
 */
char *_get_cd_path(const char *_command)
{
	const char *_ptr_space = strchr(_command, ' '); 
	char *path = NULL;
	size_t cmd_length = 0;

	if (_command == NULL)
		return (NULL);
	cmd_length = strlen(_command);
	if (cmd_length >= MAXIMUM_COMMAND_LENGTH - 1)
		return (NULL);
	if (_ptr_space == NULL)
		return (strdup ("/"));
	while (*_ptr_space == ' ')
		_ptr_space++;
	if (*_ptr_space == '\0')
		return (strdup("/"));
	path = strdup(_ptr_space);
	return (path);
}
	/**
	 * _is_wildcard-check if it is a wildcard
	 * @_command: command to be checked
	 * Return: 1 if the command is a wildcard character
	 */
	int _is_wildcard(const char *_command)
	{
		return (strpbrk((char *)_command, "*") != NULL);
	}
