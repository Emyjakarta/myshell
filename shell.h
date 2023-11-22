#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_ARGS 500

extern char **environ;

void show_prompt(void);
void read_input(void);
void remove_quotes(char *str);
void execute_command(char *cmd, char **args);
char *build_path(const char *_command[]);
void relative_path(char *cmd, char **args);

#endif /*SHELL_H*/
