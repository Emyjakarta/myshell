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
#include <errno.h>

#define MAX_COMMAND_ARGS 500
#define MAXIMUM_COMMAND_LENGTH 1000
#define MAX_OPERATORS 100
#define BUFFER_SIZE 1024

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
void obtain_operators(char **str, OperatorInfo *operators, int size);
/*OperatorInfo *obtain_operators(char **str);*/
void show_prompt(void);
char _putcharshell(const char *str);
int read_input(char **input);
void remove_quotes(char *str);
void execute_command_without_operator(char **command_copy, int *last_exit_status, OperatorInfo *current_operator);
void execute_single_command(char *command, char **arguments, int *last_exit_status, char *logical_operator);
int execute_command(char **cmd, char **args);
/*char *build_path(const char *_command);*/
void build_path(const char *_command, char *_result, size_t result_size);
int relative_path(char *cmd, char **args);
void tokenize_input(char *input, int *last_exit_status);
void tokenize_and_process_before_operator(char *before_operator, int *last_exit_status, char *current_operator);
void tokenize_and_process_after_operator(char **after_operator, char *opera, int *last_exit_status, int *total_operators);
void tokenize_and_process_last_command(char *after_operator, int *last_exit_status, char *current_operator);
void tokenize_and_process_after_operator2(int *last_exit_status, char **command_copy, OperatorInfo current_operator, char *after_operator);
void update_indices_pointers(int *operator_index, int *op_index, OperatorInfo *current_operator, OperatorInfo *operators, char **current_command, char **ops_position);
char *extract_before_operator(char **current_command, int operator_index);
char *extract_after_operator(char **current_command, int operator_index, char *operator_position, char *current_operator);
void process_operator_occurrences(int *executed_commands_count, int op_index, int *total_operators, OperatorInfo *operators, int *last_exit_status, OperatorInfo current_operator, char *original_command_copy, char *operator_position);
char *find_operator_position(char *original_command_copy, char *current_operator);
char *duplicate_command(char *command);
void process_commands_with_operators(OperatorInfo *operators, int *last_exit_status, char **command_copy, OperatorInfo *current_operator);
void cleanup(OperatorInfo *operators);
int count_operators(OperatorInfo *operators);
void _safe_free(void **ptr);
/*char *obtain_operator(char *str);*/
void trim_spaces(char *str);

int _exe_command_from_file(const char *_filename);
#endif /*SHELL_H*/
