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
#define MAXIMUM_COMMAND_LENGTH 5120
#define MAX_OPERATORS 100
#define MAX_PATH_LENGTH 5120
#define BUFFER_SIZE 5120
#define MAX_ALIASES 50
#define MAX_ALIAS_LEN 100

extern char **environ;

/**
 * struct BuiltInCommand-struct for built in commands
 * @command: command
 * @handler: handler for the commands
 * Description: struct for built in commands
 */
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
int setenv_handler(char *command, char **arguments);
int unsetenv_handler(char *command, char **arguments);
int alias_handler(char *command, char **arguments);
void print_aliases(void);
void print_alias(char *name);
void define_alias(char *name, char *value);

/**
 * struct alias-struct for aliases
 * @name: name of the alias
 * @value: value of the alias
 * Description: struct for aliases
 */
typedef struct alias
{
	char name[MAX_ALIAS_LEN];
        char value[MAX_ALIAS_LEN];
} alias;

/**
 * struct OperatorInfo-struct for retrieving logical operators
 * @operator: logical operator
 * @position: position of the logical operator
 * Description: struct for retrieving logical operator
 */
typedef struct OperatorInfo
{
	char *operator;
	int position;
} OperatorInfo;
void obtain_operators(char **str, OperatorInfo *operators, int size);
/*OperatorInfo *obtain_operators(char **str);*/
void show_prompt(void);
char _putcharshell(const char *str);
int read_input(char **argv, char **input);
void remove_quotes(char *str);
void execute_command_without_operator(const char *file_name, char **command_copy,
		int *last_exit_status, OperatorInfo *current_operator);
int execute_single_command(const char *file_name, char *command, char **arguments,
		int *last_exit_status, char *logical_operator);
int execute_command(char **cmd, char **args);
/*char *build_path(const char *_command);*/
void build_path(const char *_command, char *_result, size_t result_size);
int relative_path(char *cmd, char **args);
/*void replace_variables(char* command, char* modified_command, int *last_exit_status);*/
char *handle_variables(char *command, int *last_exit_status);
void tokenize_input(char **argv, char *input, int *last_exit_status);
void tokenize_and_process_before_operator(const char *file_name, char *before_operator,
		int *last_exit_status, char *current_operator);
void tokenize_and_process_after_operator(const char *file_name, char **after_operator,
		int *last_exit_status, int *total_operators);
int tokenize_and_process_last_command(const char *file_name, char *after_operator,
		int *last_exit_status, char *current_operator);
void tokenize_and_process_after_operator2(int *last_exit_status,
		char **command_copy, OperatorInfo current_operator, char *after_operator);
void update_indices_pointers(int *operator_index,
		int *op_index, OperatorInfo *current_operator,
		OperatorInfo *operators, char **current_command, char **ops_position);
char *extract_before_operator(char **current_command, int operator_index);
char *extract_after_operator(char **current_command,
		int operator_index, char *operator_position, char *current_operator);
int process_operator_occurrences(const char *file_name, int *total_operators, int *last_exit_status,
		OperatorInfo current_operator, char *original_command_copy,
		char *operator_position);
void process_commands_with_operators(const char *file_name, OperatorInfo *operators,
		int *last_exit_status, char **command_copy, OperatorInfo *current_operator);
void cleanup(OperatorInfo *operators);
int count_operators(OperatorInfo *operators);
void _safe_free(void **ptr);
/*char *obtain_operator(char *str);*/
void trim_spaces(char *str);

int _exe_command_from_file(char **argv);

void itoa(size_t n, char *s);
void _reverse(char *buffer, size_t len);
#endif /*SHELL_H*/
