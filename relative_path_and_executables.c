#include "shell.h"

void relative_path(char *cmd, char **args)
{
	char command_path[PATH_MAX];
	char *dir = NULL, *copy_path = getenv("PATH");
	int status = 0;
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		dir = strtok(copy_path, ":");
		while(dir != NULL)
		{
			printf("dir: %s\n", dir);
			printf("cmd: %s\n", cmd);
			fflush(stdout);
			snprintf(command_path, PATH_MAX, "%s/%s", dir, cmd);
			printf("command_path: %s\n", command_path);
			fflush(stdout);					
			if (access(command_path, X_OK) == 0)
			{
				printf("Access granted for: %s\n", command_path);
				printf("Executing: %s\n", command_path);
				printf("Executing cmd: %s\n", cmd);
				if (execve(command_path, args, NULL) == -1)
				{
					perror("execve");
					exit(EXIT_FAILURE);	
				}
			}
			dir = strtok(NULL, ":");
		}
		fprintf(stderr, "%s: not found\n", args[0]);
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
}
