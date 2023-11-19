#include "shell.h"

void execute_command(char **args)
{
	char *path = NULL,  command_path[PATH_MAX] = {0};
	char *dir = NULL, *copy_path = NULL;
	size_t dir_len, arg_len;

	if (access(args[0], F_OK) == 0)
	{
		if (execve(args[0], args, environ) == -1)
			perror("execve");
	}
	else
	{
		path = getenv("PATH");
		copy_path = strdup(path);
		dir = strtok(copy_path, ":");
		while(dir != NULL)
		{
			dir_len = strlen(dir);
			arg_len = strlen(args[0]);
			if (dir_len + 1 + arg_len >= PATH_MAX)
			{
				fprintf(stderr, "Error: PATH length exceeds maximum length\n");
				break;
			}
			strcpy(command_path, dir);
			command_path[dir_len] = '/';
			strcpy(command_path + dir_len + 1, args[0]);
			command_path[dir_len + 1 + arg_len] = '\0';
			if (access(command_path, F_OK) == 0)
			{
				if (execve(command_path, args, environ) == -1)
					perror("execve");
				break;
			}
			dir = strtok(NULL, ":");
		}
		fprintf(stderr, "%s: command not found\n", args[0]);
		exit(EXIT_FAILURE);
	}
}
