#include "shell.h"

int execute_command(char **cmd, char **args)
{
	int last_exit_status; /*signal_number;*/
	int status = 0, execve_status = 0;
	pid_t pid;
	pid_t return_pid; 

	if (cmd != NULL && access(*cmd, X_OK) == 0)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			execve_status = execve(*cmd, args, environ);
			if (execve_status == -1)
			{
				perror("execve");
				perror("Error executing command");
				last_exit_status = 99;
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			return_pid = waitpid(pid, &status, 0);
			if (return_pid == -1)
				perror("waitpid");
			if (WIFEXITED(status))
			{	
				last_exit_status = WEXITSTATUS(status);
			}
			/*else
			{
				signal_number = WTERMSIG(status);
			}*/
		}
	}
	return (last_exit_status);
}
