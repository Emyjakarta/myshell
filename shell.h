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
#include <stdbool.h>

#define MAX_COMMAND_ARGS 500
#define MAXIMUM_COMMAND_LENGTH 1000

extern char **environ;

typedef struct BuiltInCommand
{
	char *command;
	int (*handler)(char **);
} BuiltInCommand;
int builtin_handler(char *command, char **arguments);
int exit_handler(char **arguments);
int cd_handler(char **arguments);
int _is_cd(const char *_command);
char *_get_cd_path(const char *_command);
int pwd_handler(char **arguments);

void show_prompt(void);
void read_input(char *input);
void remove_quotes(char *str);
void execute_command(char *cmd, char **args);
char *build_path(const char *_command[]);
void relative_path(char *cmd, char **args);
void tokenize_input(char *input);
void _safe_free(void **ptr);

#endif /*SHELL_H*/
