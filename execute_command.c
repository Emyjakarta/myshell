#include "shell.h"

void execute_command(char *cmd, char **args)
{
	char *path = NULL,  command_path[PATH_MAX] = {0};
	char *dir = NULL, *copy_path = NULL;
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
			/*if (cmd[0] != '/')
			{*/
				path = getenv("PATH");
				printf("PATH: %s\n", path);
				fflush(stdout);
				if (path == NULL)
					perror("getenv");
				copy_path = strdup(path);
				printf("copy_path: %s\n", copy_path);
				fflush(stdout);
				if (copy_path == NULL)
					perror("strdup");
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
						if (execve(command_path, args, environ) == -1)
						{
							perror("execve");
							exit(EXIT_FAILURE);
						}
						continue;
					}
					else
					{
						    perror("Access error");
					}
					dir = strtok(NULL, ":");
				}
			/*}*/
			/*else*/
			{
				printf("Executing: %s\n", cmd);
				if (execve(cmd, args, environ) == -1)
				{
					perror("execve");
					fprintf(stderr, "%s: command not found\n", args[0]);
					exit(EXIT_FAILURE);
				}
			}
		}
		else
		{
			waitpid(pid, &status, 0);
		}
	}
}
