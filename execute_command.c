#include "shell.h"

int execute_command(char *cmd, char **args)
{
	int last_exit_status, signal_number;
	int status = 0, execve_status = 0;
	pid_t pid;
	pid_t return_pid; 

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
	printf("Before access check. cmd: %s\n", cmd);
	if (cmd != NULL && access(cmd, X_OK) == 0)
	{
		pid = fork();
		printf("After fork call. pid: %d\n", pid);
		if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			printf("Executing just before calling execve: %s\n", cmd);
			execve_status = execve(cmd, args, environ);
			if (execve_status == -1)
			{
				printf("execve_status: %d\n", execve_status);
				perror("execve");
				perror("Error executing command");
				last_exit_status = 99;
				printf("After execution, last_exit_status = %d\n", last_exit_status);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			printf("Entering the else block.\n");
			/*waitpid(pid, &status, 0);*/
			return_pid = waitpid(pid, &status, 0);
			if (return_pid == -1)
				perror("waitpid");
			printf("Before WIFEXITED(status) check\nstatus: %d\nWEXITSTATUS(status): %d\n", status, WEXITSTATUS(status));
			if (WIFEXITED(status))
			{	
				last_exit_status = WEXITSTATUS(status);
				printf("After execution, last_exit_status = %d\n", last_exit_status);
			}
			else
			{
				signal_number = WTERMSIG(status);
				printf("Process was terminated by signal %d\n", signal_number);
			}
		}
	}
	return (last_exit_status);
	/*}*/
}
