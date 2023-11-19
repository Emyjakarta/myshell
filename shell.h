#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_COMMAND_ARGS 500

extern char **environ;

void show_prompt(void);
void read_input(char *input);
void remove_quotes(char *str);
void execute_command(char **args);

#endif /*SHELL_H*/
