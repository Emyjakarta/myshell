#include "shell.h"

void execute_command(char *cmd, char **args)
{
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
			printf("Executing: %s\n", cmd);
			if (execve(cmd, args, environ) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else
			waitpid(pid, &status, 0);
	}
	fprintf(stderr, "%s: not found\n", args[0]);
}
