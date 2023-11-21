#include "shell.h"

void execute_command(char *cmd, char **args)
{
	char *path = NULL,  command_path[PATH_MAX] = {0};
	char *dir = NULL, *copy_path = NULL;
	/*size_t dir_len, arg_len;*/
	/*size_t last_char_index;*/
	/*char *delim = " \t", *token = NULL;*/
	/*	char *com[MAX_COMMAND_ARGS] = strdup(*args);*/
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
			if (cmd[0] != '/')
			{
				path = getenv("PATH");
				copy_path = strdup(path);
				dir = strtok(copy_path, ":");
				while(dir != NULL)
				{
					/*dir_len = strlen(dir);
					arg_len = strlen(args[0]);*/
					/*if (dir_len + 1 + arg_len >= PATH_MAX)
					{
						fprintf(stderr, "Error: PATH length exceeds maximum length\n");
						break;
					}*/
					printf("dir: %s\n", dir);
					printf("cmd: %s\n", cmd);
					fflush(stdout);
					snprintf(command_path, PATH_MAX, "%s/%s", dir, cmd);
					/*strcpy(command_path, dir);*/
					/*last_char_index = strlen(command_path);*/
				/*	strcat(command_path, "/");
					strcat(command_path, cmd);*/
					/*command_path[dir_len + 1 + arg_len] = '\0';*/
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
						/*	continue;*/
						}
						continue;
					}
					else
					{
						    perror("Access error");
					}
					dir = strtok(NULL, ":");
				}
			}
			else
			{
				printf("Executing: %s\n", cmd);
				if (execve(cmd, args, environ) == -1)
				{
					perror("execve");
					fprintf(stderr, "%s: command not found\n", args[0]);
					exit(EXIT_FAILURE);
				}
			}
			/*fprintf(stderr, "%s: command not found\n", args[0]);
			exit(EXIT_FAILURE);*/
		}
		else
		{
			waitpid(pid, &status, 0);
		}
	}
}
