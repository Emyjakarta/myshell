#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <sys/stat.h>
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
	int (*handler)(char *, char **);
} BuiltInCommand;
int builtin_handler(char *command, char **arguments);
int exit_handler(char *command, char **arguments);
int cd_handler(char *command, char **arguments);
int _is_cd(const char *_command);
char *_get_cd_path(const char *_command, char **arguments);
int pwd_handler(char *command, char **arguments);

typedef struct {
	char *operator;
	int position;
} OperatorInfo;
OperatorInfo *obtain_operators(char *str);
void show_prompt(void);
char _putcharshell(const char *str);
void read_input(char *input);
void remove_quotes(char *str);
int execute_single_command(char *command, char **arguments, int result, char *logical_operator);
int execute_command(char *cmd, char **args);
char *build_path(const char *_command[]);
int relative_path(char *cmd, char **args);
int tokenize_input(char *input);
void _safe_free(void **ptr);
/*char *obtain_operator(char *str);*/
void trim_spaces(char *str);

#endif /*SHELL_H*/
