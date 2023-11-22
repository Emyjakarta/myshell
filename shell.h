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
#include <stdarg.h>

#define MAX_COMMAND_ARGS 500

extern char **environ;

typedef struct BuiltInCommand
{
	char *command;
	int (*handler)();
} BuiltInCommand;
int builtin_handler(char *command, va_list ptr);
int exit_handler(void);
int cd_handler(va_list ptr);
int _is_cd(const char *_command);
int pwd_handler(void);

void show_prompt(void);
void read_input(char *input, ...);
void remove_quotes(char *str);
void execute_command(char *cmd, char **args);
char *build_path(const char *_command[]);
void relative_path(char *cmd, char **args);
void tokenize_input(char *input, ...);

#endif /*SHELL_H*/
