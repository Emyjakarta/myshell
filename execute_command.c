#include "shell.h"

void execute_command(char *cmd, char **args)
{
	char *path = NULL,  command_path[PATH_MAX] = {0};
	char *dir = NULL, *copy_path = NULL;
	size_t dir_len, arg_len;
	/*char *delim = " \t", *token = NULL;*/
	/*	char *com[MAX_COMMAND_ARGS] = strdup(*args);*/
	int status = 0;
	pid_t pid;

	if (access(cmd, X_OK) == 0)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (cmd[0] != '/')
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
					dir_len = strlen(dir);
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
			}
			else
			{
				if (execve(cmd, args, environ) == -1)
				{
					perror("execve");
					exit(EXIT_FAILURE);
				}
			}
			fprintf(stderr, "%s: command not found\n", args[0]);
			exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(pid, &status, 0);
		}
	}
}
