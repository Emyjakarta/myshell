#include "shell.h"

void execute_command(char *cmd, char **args, int *last_exit_status)
{
	int status = 0, execve_status = 0;
	pid_t pid;

	/*if (strcmp(cmd, "&&") == 0)
	  {
	  if (*last_exit_status == 0) {
	  execute_command(args[0], args, last_exit_status);
	  }
	  } else if (strcmp(cmd, "||") == 0) {
	  if (*last_exit_status != 0) {
	  execute_command(args[0], args, last_exit_status);
	  }
	  } else {*/
	if (cmd != NULL && access(cmd, X_OK) == 0)
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
			execve_status = execve(cmd, args, environ);
			if (execve_status == -1)
			{
				printf("execve_status: %d\n", execve_status);
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
			{	*last_exit_status = WEXITSTATUS(status);
				printf("After execution, last_exit_status = %d\n", *last_exit_status);
			}
			else
			{
				fprintf(stderr, "%s: not found\n", args[0]);
				*last_exit_status = -1;
			}
		}
	}
	/*}*/
}
