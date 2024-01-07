#include "shell.h"
/**
 * read_input-read input
 * @input: input
 * Return: 0
 */
int read_input(char **input)
{
	ssize_t read = 0;
	size_t buffer = 0;
	char *comment_pos = NULL;
	int last_exit_status = 0;

	read = getline(input, &buffer, stdin);
	if (read == -1)
	{
		if (errno == ENOMEM)
		{
			fprintf(stderr, "malloc failed\n");
			return (1);
		}
	}
	printf("input after getline: %s\n", *input);
	(*input)[strcspn(*input, "\n")] = '\0';
	comment_pos = strchr(*input, '#');
	if (comment_pos != NULL)
		*comment_pos = '\0';
	remove_quotes(*input);
	tokenize_input(*input, &last_exit_status);
	return (0);
}
