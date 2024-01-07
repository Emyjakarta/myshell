#include "shell.h"
/**
 * _exe_command_from_file-execute command from file
 * @_filename: name of file
 * Return: void
 */
int _exe_command_from_file(char **argv)
{
	char *_line = NULL, *comment_pos = NULL;
	size_t _len = BUFFER_SIZE;
	ssize_t _read;
	FILE *_file = fopen(argv[1], "r");
	int last_exit_status = 0;

	if (_file == NULL)
	{
		/*if (fd == -1)
		{*/
			/* we couldn't open the file, let's clean and leave */
			/*free_list(&path_list);*/
			dprintf(2, "%s: 0: Can't open %s\n", argv[0], argv[1]);
			return (127);
		/*}*/
		/*perror("File opening failed");
		  exit(EXIT_FAILURE);*/
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
		_line[strcspn(_line, "\n")] = '\0';
		comment_pos = strchr(_line, '#');
		if (comment_pos != NULL)
			*comment_pos = '\0';
		remove_quotes(_line);
		tokenize_input(argv, _line, &last_exit_status);
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
