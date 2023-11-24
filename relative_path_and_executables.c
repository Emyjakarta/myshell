#include "shell.h"

void relative_path(char *cmd, char **args, int *last_exit_status)
{
	char command_path[PATH_MAX];
	char *dir = NULL, *copy_path = getenv("PATH");
	int status = 0;
	pid_t pid;

	if (last_exit_status != NULL)
	{
		if (strcmp(cmd, "&&") == 0) {
			if (*last_exit_status == 0) {
				relative_path(cmd, args, last_exit_status);
			}
		} else if (strcmp(cmd, "||") == 0) {
			if (*last_exit_status != 0) {
				relative_path(cmd, args, last_exit_status);
			}
		}
	} else {
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
					if (execve(command_path, args, environ) == -1)
					{
						perror("execve");
						exit(EXIT_FAILURE);	
					}
				}
				dir = strtok(NULL, ":");
			}
			fprintf(stderr, "%s: not found\n", args[0]);
			exit(EXIT_FAILURE);
			/*last_exit_status = 1;*/
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				*last_exit_status = WEXITSTATUS(status);
			else
				*last_exit_status = -1;
		}
	}
}
