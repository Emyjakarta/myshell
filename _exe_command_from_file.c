#include "shell.h"
/**
 * _exe_command_from_file-execute command from file
 * @_filename: name of file
 * Return: void
 */
int _exe_command_from_file(const char *_filename)
{
	char *_line = NULL;
	size_t _len = BUFFER_SIZE;
	ssize_t _read;
	FILE *_file = fopen(_filename, "r");
	int last_exit_status = 0;
	char *comment_pos = NULL;

	if (_file == NULL)
	{
		perror("File opening failed");
		exit(EXIT_FAILURE);
	}
	_line = calloc(_len, sizeof(char));
	if (_line == NULL)
	{
		fprintf(stderr, "Error: malloc failed\n");
		fclose(_file);
		exit(EXIT_FAILURE);
	}
	while ((_read = getline(&_line, &_len, _file)) != -1)
	{
		printf("Read: %ld\n", _read);
		printf("_line after getline: %s\n", _line);
		_line[strcspn(_line, "\n")] = '\0';
		comment_pos = strchr(_line, '#');
		if (comment_pos != NULL)
			comment_pos = '\0';
		remove_quotes(_line);
		tokenize_input(_line, &last_exit_status);
	}
	if (_read == -1)
	{
		if (errno == ENOMEM)
		{
			fprintf(stderr, "malloc failed");
			return (1);
		}
	}
	fclose(_file);
	free(_line);
	return (0);
}
