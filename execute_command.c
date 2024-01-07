#include "shell.h"
/**
 * execute_command_without_operator-execute command without operator
 * @command_copy: command copy
 * @last_exit_status: last exit status
 * @current_operator: current operator
 */
void execute_command_without_operator(char **command_copy,
		int *last_exit_status, OperatorInfo *current_operator)
{
	char *arg_token, *command_args[MAX_COMMAND_ARGS];
	char *saveptr1 = NULL;
	int arg_count;

	arg_count = 0;
	arg_token = strtok_r(*command_copy, " \t", &saveptr1);
	while (arg_token != NULL && arg_count < MAX_COMMAND_ARGS - 1)
	{
		command_args[arg_count++] = arg_token;
		arg_token = strtok_r(NULL, " \t", &saveptr1);
	}
	command_args[arg_count] = NULL;
	execute_single_command(command_args[0], command_args,
			&(*last_exit_status), current_operator->operator);
}
/**
 * execute_single_command-execute single command
 * @command: command
 * @arguments: arguments
 * @last_exit_status: last exit status
 * @logical_operator: logical operator
 */
void execute_single_command(char *command, char **arguments,
		int *last_exit_status, char *logical_operator)
{
	int b_result = 0;
	char command_buffer[PATH_MAX];
	(void) logical_operator;

	command_buffer[0] = '\0'; /* Initialize buffer */
	b_result = builtin_handler(command, arguments);
	if (b_result == 1)
	{
		if (command != NULL && *arguments[0] != '/')
		{
			build_path(command, command_buffer, PATH_MAX);
			if (command_buffer[0] == '\0')
			{
				fprintf(stderr, "%s: not found\n", command);
				*last_exit_status = 5;
				return;
			}
			command = command_buffer; /* Update command to point to the result */
		}
		*last_exit_status = execute_command(&command, arguments);
	}
}
/**
 * execute_command-execute command
 * @cmd: command
 * @args: arguments
 * Return: last exit status
 */
int execute_command(char **cmd, char **args)
{
	int last_exit_status;
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
			(return_pid = waitpid(pid, &status, 0));
			if (return_pid == -1)
				perror("waitpid");
			if (WIFEXITED(status))
			{
				last_exit_status = WEXITSTATUS(status);
			}
		}
	}
	return (last_exit_status);
}
