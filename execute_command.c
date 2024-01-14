#include "shell.h"
/**
 * execute_command_without_operator-execute command without operator
 * @command_copy: command copy
 * @last_exit_status: last exit status
 * @current_operator: current operator
 */
void execute_command_without_operator(const char *file_name, char **command_copy,
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
	*last_exit_status = execute_single_command(file_name, command_args[0], command_args,
			&(*last_exit_status), current_operator->operator);
}
/**
 * execute_single_command-execute single command
 * @command: command
 * @arguments: arguments
 * @last_exit_status: last exit status
 * @logical_operator: logical operator
 * Return: last exit status or 0
 */
int execute_single_command(const char *file_name, char *command, char **arguments,
		int *last_exit_status, char *logical_operator)
{
	int b_result = 0;
	static size_t err_count = 1;
	char command_buffer[PATH_MAX];
	char *actual_command = NULL;
	struct stat file_info;
	/*char *modified_command = NULL;*/
	(void) logical_operator;

	command_buffer[0] = '\0'; /* Initialize buffer */
	b_result = builtin_handler(command, arguments);
	if (b_result == 1)
	{
		is_command_in_path(command, arguments, &(*last_exit_status));
		printf("last_exit_status: %d\n", *last_exit_status);
		printf("command after is_command: %s\n", command);
		printf("arguments[0] after is_command: %s\n", arguments[0]);
		printf("arguments[0][0] after is_command: %c\n", arguments[0][0]);
		printf("arguments[0][1] after is_command: %c\n", arguments[0][1]);
		printf("access result: %d\n", access(command, F_OK));
		if (access(command, F_OK) == -1 && (arguments[0][0] == '.' && arguments[0][1] == '.'))
		{
			dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", file_name, err_count, arguments[0]);
			err_count++;
			*last_exit_status = 127;
			return (*last_exit_status);
		}
		if(*last_exit_status == 1 && ((arguments[0][0] != '.'  && arguments[0][1] != '/')))
		{
			/*if (command != NULL && (arguments[0][0] != '.' || (strcmp(arguments[0], "..") != 0)))*/
			/*|| !(arguments[0][    0] == '.' && arguments[0][1] == '.')))*/

			if (arguments[0][0] != '/')
			{
				printf("1\n");
				printf("command for 1: %s\n", command);
				/*printf("value of arguments[0] in strncmp: %d\n", strncmp(arguments[0], "..", 2));*/
				printf("arguments[0] for 1: %s\n", arguments[0]);
				printf("arguments[0][0] for 1: %c\n", arguments[0][0]);
				printf("arguments[0][1] for 1: %c\n", arguments[0][1]);
				printf("access result for 1: %d\n", access(command, F_OK));
				dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", file_name, err_count, arguments[0]);
				err_count++;
				*last_exit_status = 127;
				return (*last_exit_status);
			}
		}
		else if (*last_exit_status == 1 && ((arguments[0][0] == '.' && arguments[0][1] == '/')))
			/*else if (*last_exit_status == 1 && (arguments[0][0] == '.' || (strncmp(arguments[0], "..", 2) == 0)) && access(command, F_OK) == 0)*/
		{
			if (access(command, F_OK) != 0)
			{
				dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", file_name, err_count, arguments[0]);
				err_count++;
				*last_exit_status = 127;
				return (*last_exit_status);
			}
			else
			{
				actual_command = arguments[0] + 2;  /* Skip the "./" prefix */
				printf("actual_command after assignment: %s\n", actual_command);
				*last_exit_status = execute_command(&actual_command, arguments);
				return (*last_exit_status);
			}
		}
		else if (*last_exit_status == 1 && ((arguments[0][0] != '.' && arguments[0][1] != '.')))
		{
			dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", file_name, err_count, arguments[0]);
			err_count++;
			*last_exit_status = 127;
			return (*last_exit_status);
		}
		else if (*last_exit_status == 1 && ((arguments[0][0] == '.' && arguments[0][1] == '.')))
		{
			actual_command = arguments[0] + 3;  /* Skip the "../" prefix */
			printf("command for 4: %s\n", command);
			printf("actual_command after assignment: %s\n", actual_command);
			printf("access result(F_OK) for 4: %d\n", access(command, F_OK));
			printf("access result(X_OK) for 4: %d\n", access(command, X_OK));
			printf("stat(command, &file_info) for 4: %d\n", stat(command, &file_info));
			printf("S_ISREG(file_info.st_mode) for 4: %d\n", S_ISREG(file_info.st_mode));
			printf("(file_info.st_mode & S_IXUSR) for 4: %d\n", (file_info.st_mode & S_IXUSR));
			if (stat(command, &file_info) == 0 && S_ISREG(file_info.st_mode) && (file_info.st_mode & S_IXUSR))
			{
				*last_exit_status = execute_command(&command, arguments);
				return (*last_exit_status);
			}
			else
			{
				dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", file_name, err_count, arguments[0]);
				err_count++;
				*last_exit_status = 127;
				return (*last_exit_status);
			}
		}


		/*else
		  return (*last_exit_status);*/
		if (command != NULL && *arguments[0] != '/')
		{
			/*if (is_command_in_path(command, arguments, &(*last_exit_status)) != 0)
			  {
			 *last_exit_status = execute_command(&command, arguments);
			 return (*last_exit_status);
			 }*/


			printf("last_exit_status before build_path: %d\n", *last_exit_status);
			build_path(command, command_buffer, PATH_MAX);
			if (command_buffer[0] == '\0' && access(command, F_OK) != 0)
			{
				printf("command after build_path during check: %s\n", command);
				printf("command_buffer after build_path during check: %s\n", command_buffer);
				dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", file_name, err_count,
						arguments[0]);
				err_count++;
				*last_exit_status = 127;
				return (*last_exit_status);
				/*exit(127);*/
			}
			/*modified_command = command_buffer;*/
			/*printf("command after build_path: %s\n", command);
			  printf("command_buffer after build_path: %s\n", command_buffer);
			 *last_exit_status = execute_command(&modified_command, arguments);
			 return (*last_exit_status);*/

			/*else
			  modified_command = command;
			  printf("command after assignment: %s\n", command);
			  printf("modified_command after assignment: %s\n", modified_command);
			 *last_exit_status = execute_command(&modified_command, arguments);
			 return (*last_exit_status);*/
			/*command = command_buffer;*/ /* Update command to point to the result */
		}
		else
		{
			if (command != NULL && (arguments[0][0] == '.' || (strcmp(arguments[0], "..") == 0)) && access(command, F_OK) != 0)
			{
				printf("two\n");
				dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", file_name, err_count, arguments[0]);
				err_count++;
				*last_exit_status = 127;
				return (*last_exit_status);
			}
			else if (command != NULL && *arguments[0] == '/' && access(command, F_OK) != 0)
			{
				dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", file_name, err_count,
						arguments[0]);
				err_count++;
				/*exit(127);*/
				*last_exit_status = 127;
				return (*last_exit_status);
			}
			*last_exit_status = execute_command(&command, arguments);
		}
		/*last_exit_status = execute_command(&command, arguments);*/
	}
	return (*last_exit_status);
}
/***last_exit_status = execute_command(&command, arguments);
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
