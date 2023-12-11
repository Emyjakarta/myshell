#include "shell.h"

int relative_path(char *cmd, char **args)
{
	int last_exit_status = 0;
	char command_path[PATH_MAX];
	char *dir = NULL, *copy_path = getenv("PATH");
	int status = 0, execve_status = 0, i = 0;
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
				printf("contents of the args array before execve:\n");
				for (i = 0; args[i] != NULL; i++)
				{
					    printf("args[%d]: %s\n", i, args[i]);
				}
				execve_status = execve(command_path, args, environ);
				if (execve_status == -1)
				{
					printf("execve_status: %d\n", execve_status);
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
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			last_exit_status = WEXITSTATUS(status);
			printf("After execution, last_exit_status = %d\n", last_exit_status);
		}
		else
			last_exit_status = -1;
	}
	return (last_exit_status);
}
