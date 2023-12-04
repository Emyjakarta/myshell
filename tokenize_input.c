#include "shell.h"
/*bool skip_next_command = false;*/

int tokenize_input(char *input)
{
	int last_exit_status = 0;
	char *arg_token = NULL, *delim = ";\n", *arg_token_op = NULL, *arg_token_op_aft = NULL;
	int index = 0, i = 0, arg_count = 0, arg_count_op = 0, arg_count_op_aft = 0;
	char *command_args[MAX_COMMAND_ARGS], *command_args_op[MAX_COMMAND_ARGS], *command_args_op_aft[MAX_COMMAND_ARGS];
	char *arg = NULL, *single_command = NULL; /*arg_op = NULL;*/
	char *saveptr1 = NULL, *saveptr2 = NULL;
	char *logical_operator = NULL;
	char *command_copy = NULL;
	char *before_operator = NULL;
	char *after_operator = NULL;
	size_t operator_index = 0;
	char *operator_location = NULL;

	single_command = strtok_r(input, delim, &saveptr1);
	while (single_command != NULL && index < MAX_COMMAND_ARGS - 1)
	{
		command_copy = strdup(single_command);
		if (command_copy == NULL)
			return (50);
		logical_operator = obtain_operator(command_copy);
		if (logical_operator == NULL)
		{
			printf("Next command: %s\n", saveptr1);
			printf("single command: %s\nsaveptr1: %s\n", command_copy, saveptr1);
			arg_count = 0;
			arg_token = strtok_r(command_copy, " \t", &saveptr2);
			printf("arg_token: %s\nsaveptr2: %s\n", arg_token, saveptr2);
			while (arg_token != NULL && arg_count < MAX_COMMAND_ARGS - 1)
			{                                                             
				arg = strdup(arg_token);
				if (arg == NULL)
					perror("strdup");
				command_args[arg_count++] = arg;
				arg_token = strtok_r(NULL, " \t", &saveptr2);
			}
			command_args[arg_count] = NULL;
			printf("contents of the command_args array after inner while loop:\n");
			for (i = 0; i < arg_count; i++)
			{
				printf("command_args[%d]: %s\n", i, command_args[i]);
			}
			last_exit_status = execute_single_command(command_args[0], command_args, last_exit_status, logical_operator);
		}
		else
		{
			operator_location = strstr(command_copy, logical_operator);
		}
		if (logical_operator != NULL)
		{
			before_operator = malloc(strlen(command_copy) + 1);
			if (before_operator == NULL)
				return (19);
			after_operator = malloc(strlen(command_copy) + 1);
			if (after_operator == NULL)
				return (20);
			operator_index = operator_location - command_copy;
			if (operator_index > strlen(command_copy))
				return (52);
			strncpy(before_operator, command_copy, operator_index);
			before_operator[operator_index] = '\0';
			printf("before_operator: %s\n", before_operator);
			if (operator_index + strlen(logical_operator) > strlen(command_copy))
				return (53);
			strncpy(after_operator, command_copy + operator_index + strlen(logical_operator), strlen(command_copy) - operator_index - strlen(logical_operator) + 1);
			after_operator[strlen(command_copy) - operator_index - strlen(logical_operator)] = '\0';
			printf("after_operator: %s\n", after_operator);
			trim_spaces(before_operator);
			trim_spaces(after_operator);

			printf("Next command_op: %s\n", saveptr1);
			printf("single command_op: %s\nsaveptr1: %s\n", command_copy, saveptr1);
			arg_count_op = 0;
			arg_token_op = strtok_r(before_operator, " \t", &saveptr2);
			printf("arg_token_op: %s\nsaveptr2: %s\n", arg_token_op, saveptr2);
			while (arg_token_op != NULL && arg_count_op < MAX_COMMAND_ARGS - 1)
			{
				if (strcmp(arg_token_op, "&&") == 0 || strcmp(arg_token_op, "||") == 0)
				{
					arg_token_op = strtok_r(NULL, " \t", &saveptr2);
					continue;
				}
				/*arg_op = strdup(arg_token_op);*/
				/*if (arg_op == NULL)
				  perror("strdup");*/
				command_args_op[arg_count_op++] = strdup(arg_token_op);
				arg_token_op = strtok_r(NULL, " \t", &saveptr2);
			}
			command_args_op[arg_count_op] = NULL;
			arg_count_op_aft = 0;
			command_args_op_aft[0] = NULL;
			arg_token_op_aft = strtok_r(after_operator, " \t", &saveptr2);
			printf("arg_token_op_aft: %s\nsaveptr2: %s\n", arg_token_op_aft, saveptr2);
			while (arg_token_op_aft != NULL && arg_count_op_aft < MAX_COMMAND_ARGS - 1)
			{
				if (strcmp(arg_token_op_aft, "&&") == 0 || strcmp(arg_token_op_aft, "||") == 0)
				{
					arg_token_op_aft = strtok_r(NULL, " \t", &saveptr2);
					continue;
				}
				command_args_op_aft[arg_count_op_aft++] = strdup(arg_token_op_aft);
				arg_token_op_aft = strtok_r(NULL, " \t", &saveptr2);
			}
			command_args_op_aft[arg_count_op_aft] = NULL;
			printf("contents of the command_args_op_aft array after inner while loop:\n");
			for (i = 0; i < arg_count_op_aft; i++)
			{
				printf("command_args_op_aft[%d]: %s\n", i, command_args_op_aft[i]);
			}
			/*last_exit_status = execute_single_command(command_args[0], command_args, last_exit_status, logical_operator);*/

			if (strcmp(logical_operator, "&&") == 0)
			{
				printf("Execute command before '&&': %s\n", before_operator);
				last_exit_status = execute_single_command(before_operator, command_args_op, last_exit_status, logical_operator);
				if (last_exit_status == 0)
				{
					printf("Execute command after '&&': %s\n", after_operator);
					last_exit_status = execute_single_command(after_operator, command_args_op_aft, last_exit_status, logical_operator);
				}
			}
			else if (strcmp(logical_operator, "||") == 0)
			{
				printf("Execute command before '||': %s\n", before_operator);
				last_exit_status = execute_single_command(before_operator, command_args_op, last_exit_status, logical_operator);
				if (last_exit_status != 0)
				{
					printf("Execute command after '||': %s\n", after_operator);
					last_exit_status = execute_single_command(after_operator, command_args_op_aft, last_exit_status, logical_operator);
				}
			}
			free(before_operator);
			free(after_operator);
		}
		/*else
		  {
		  printf("Next command: %s\n", saveptr1);
		  printf("single command: %s\nsaveptr1: %s\n", command_copy, saveptr1);
		  arg_count = 0;
		  arg_token = strtok_r(command_copy, " \t", &saveptr2);
		  printf("arg_token: %s\nsaveptr2: %s\n", arg_token, saveptr2);
		  while (arg_token != NULL && arg_count < MAX_COMMAND_ARGS - 1)
		  {
		  arg = strdup(arg_token);
		  command_args[arg_count++] = arg;
		  arg_token = strtok_r(NULL, " \t", &saveptr2);
		  }
		  command_args[arg_count] = NULL;
		  }
		  last_exit_status = execute_single_command(command_args[0], command_args, last_exit_status, logical_operator);*/
		/*		printf("logical operator before check: %s\n", logical_operator);
				printf("last_exit_status before check: %d\n", last_exit_status);*/
		/*if (skip_next_command)
		  {
		  skip_next_command = false;
		  continue;
		  }*/
		/*if (logical_operator != NULL) {
		  if ((strcmp(logical_operator, "&&") == 0 && last_exit_status != 0) ||
		  (strcmp(logical_operator, "||") == 0 && last_exit_status == 0)) {
		  printf("Skipping command due to logical operator: %s\n", logical_operator);
		  skip_next_command = true;
		  }
		  }*/
		/*if (skip_next_command)
		  {
		  skip_next_command = false;
		  continue;
		  }*/
		/*		last_exit_status = execute_single_command(command_args[0], command_args, last_exit_status, logical_operator);*/
		/*logical_operator = logical_operator_next;
		  logical_operator_next = NULL;*/
		/*if (last_exit_status == 9)
		  {
		  last_exit_status = 9;
		  printf("Command was skipped, last exit status = %d\n", last_exit_status);
		  }
		  else
		  {
		  printf("Command was executed, last exit status = %d\n", last_exit_status);
		  }*/
		/*if ((len >= 2 && single_command[len - 2] == '&' && single_command[len - 1] == '&')
		  || (len >= 2 && single_command[len - 2] == '|' && single_command[len - 1] == '|'))
		  {
		  com = strtok_r(single_command, "&&||", &saveptr1);*
		  logical_operator = strdup("&&||");
		  single_command[len - 2] = '\0'; Remove the logical operator
		  }*/
		/*logical_and = strstr(single_command, "&&");
		  logical_or = strstr(single_command, "||");
		  if (logical_and != NULL && (logical_or == NULL || logical_and < logical_or)) {
		  logical_operator = "&&";
		  next_command = logical_and + 2;
		  } else if (logical_or != NULL) {
		  logical_operator = "||";
		  next_command = logical_or + 2;
		  }
		  else
		  {
		  logical_operator = NULL;
		  next_command = '\0';
		  }

		  if (logical_operator != NULL) {
		 *next_command = '\0';  *Split the command at the operator*/
		/*next_command += 2;*/     /*Move to the next command after the operator*/
		/*}*/

		/*Trim leading/trailing spaces from the command*/
		/*while (*command && isspace(*command)) {
		  command++;
		  }*/
		/*int len = strlen(command);
		  while (len > 0 && isspace(command[len - 1])) {
		  command[--len] = '\0';
		  }*/

		/*len = strlen(command_copy);
		  while (len > 0 && (command_copy[len - 1] == ' ' || command_copy[len - 1] == '\t')) 
		  {
		  command_copy[--len] = '\0';
		  }
		  printf("Trimmed single command: %s\n", command_copy);*/
		/*if (strncmp(single_command, "&&", 2) == 0 || strncmp(single_command, "||", 2) == 0)
		  {
		  logical_operator = (char *)malloc(3 * sizeof(char));
		  strncpy(logical_operator, single_command, 2);
		  logical_operator[2] = '\0'; *Null-terminate the string*/

		/*Skip the operator by adding 2 to single_command*/
		/*single_command += 2;
		  }
		  else
		  {
		  input = NULL;
		  }*/
		/*printf("Next command: %s\n", saveptr1);
		  printf("single command: %s\nsaveptr1: %s\n", command_copy, saveptr1);
		  arg_count = 0;
		  arg_token = strtok_r(command_copy, " \t", &saveptr2);
		  printf("arg_token: %s\nsaveptr2: %s\n", arg_token, saveptr2);
		  while (arg_token != NULL && arg_count < MAX_COMMAND_ARGS - 1)
		  {
		  arg = strdup(arg_token);*/
		/*arg[strlen(arg_token)] = '\0';*/
		/*command_args[arg_count++] = arg;
		  arg_token = strtok_r(NULL, " \t", &saveptr2);*/
		/*printf("arg_tokenb: %s\nsaveptr2b: %s\n", arg_token, saveptr2);*/
		/*}
		  command_args[arg_count] = NULL;*/
		/*printf("Before checking operators: %s\n", arg_token);
		  if (arg_token != NULL && (strcmp(arg_token, "&&") == 0 || strcmp(arg_token, "||") == 0))
		  {
		  if (strcmp(arg_token, "&&") == 0)
		  {
		  if (*last_exit_status == 0)
		  {
		  printf("last_exit_status: %d\n", *last_exit_status);
		  printf("command_args[0]: %s\n", command_args[0]);
		  execute_single_command(command_args[0], command_args, last_exit_status);
		  }
		  }
		  else if (strcmp(arg_token, "||") == 0)
		  {
		  if (*last_exit_status != 0)
		  {
		  printf("last_exit_status: %d\n", *last_exit_status);
		  printf("command_args[0]: %s\n", command_args[0]);
		  execute_single_command(command_args[0], command_args, last_exit_status);
		  }
		  }
		  printf("After checking operators: %s\n", arg_token);
		  for (i = 0; i < arg_count; i++) {
		  free(command_args[i]);
		  command_args[i] = NULL;
		  }
		  single_command = strtok_r(NULL, delim, &saveptr1);*/
		/*}*/
		/*else
		  {)}*/
		/*if (logical_operator == NULL ||
		  (strcmp(logical_operator, "&&") == 0 && last_exit_status == 0) ||
		  (strcmp(logical_operator, "||") == 0 && last_exit_status != 0)) {*/
		/*}*/
		/*execute_single_command(command_args[0], command_args, last_exit_status, logical_operator);*/
		/*store exit status of the command in last_exit_status*/

		/*if (logical_operator != NULL) {
		  free(logical_operator);
		  logical_operator = NULL;
		  }*/
		/*if (logical_operator_next != NULL) {
		  free(logical_operator_next);
		  logical_operator_next = NULL;
		  }*/
		for (i = 0; i < arg_count; i++) {
			free(command_args[i]);
			command_args[i] = NULL;
		}
		/*free(command_copy);*/
		single_command = strtok_r(NULL, delim, &saveptr1);
		index++;

		/*single_command = next_command;*/
	}
	return (last_exit_status);
}

/*single_command = strtok_r(NULL, delim, &saveptr1);*/
/*for (i = 0; i < arg_count; i++) {
  free(command_args[i]);
  command_args[i] = NULL;
  }*/
int execute_single_command(char *command, char **arguments, int result, char *logical_operator)
{
	printf("Before execution, last_exit_status = %d\n", result);
	/*printf("Executing Command: %s\n", command);*/
	printf("Logical operator: %s\n", logical_operator);
	/*if (logical_operator != NULL) {
	  if ((strcmp(logical_operator, "&&") == 0 && result != 0) ||
	  (strcmp(logical_operator, "||") == 0 && result == 0)) {
	  printf("Skipping command due to logical operator: %s\n", logical_operator);
	  skip_next_command = true;
	  }
	  printf("Skip_next_command has been set to true\n");
	  printf("Before calling builtin (line 212), logical_operator: %s\n", logical_operator);
	  printf("Before calling builtin (line 212), result: %d\n", result);
	  printf("Before calling builtin (line 212), skip_next_command: %d\n", skip_next_command);
	  }
	  else if (logical_operator == NULL ||
	  (strcmp(logical_operator, "&&") == 0 && result == 0) ||
	  (strcmp(logical_operator, "||") == 0 && result != 0)) {*/
	result = builtin_handler(command, arguments);
	printf("result of builtin_handler: %d\n", result);
	if (result == 1) {
		if (command != NULL && command[0] != '/') {
			result = relative_path(command, arguments);
		} else {
			result = execute_command(command, arguments);
		}
		printf("After executing commands, result in execute_single_command: %d\n", result);
	}
	/*}*/
	return (result);
}
/*printf("Executing Command: %s\n", command);*/
/*arguments[0] = command;*/
/*for (i = 0; i < arg_count; i++)
  {
  free(command_args[i]);*/
/*free(command_args[i]);*/
/*command_args[i] = NULL;
  }*/
/*printf("single_command: %s\n", single_command);*/
/*single_command = strtok_r(NULL, delim, &saveptr1);
  printf("single_command: %s\nsaveptr1: %s\n", single_command, saveptr1);*/

/*else
  {
  printf("Executing Command: %s\n", command_args[0]);
  result = builtin_handler(command_args[0], command_args + 1, last_exit_status);
  if (result != 4) {
  if (command_args[0][0] != '/') {
  relative_path(command_args[0], command_args, last_exit_status);
  } else {
  execute_command(command_args[0], command_args, last_exit_status);
  }
  }
 *printf("Executing Command: %s\n", command_args[0]);
 builtin_handler(command_args[0], command_args + 1);
 if (command_args[0][0] != '/')
 {
 relative_path(command_args[0], command_args);
 }
 else
 execute_command(command_args[0], command_args);*/
/*printf("Executing Command: %s\n", command_args[0]);
  for (i = 0; i < arg_count; i++)
  {
  free(command_args[i]);
  free(command_args[i]);*
  command_args[i] = NULL;
  }
  printf("single_command: %s\n", single_command);
  single_command = strtok_r(NULL, delim, &saveptr1);
  printf("single_command: %s\nsaveptr1: %s\n", single_command, saveptr1);
  }
  }*/
/*for (i = 0; i < arg_count; i++)
  {
  _safe_free((void **)&command_args[i]);
 *free(command_args[i]);*/
/*command_args[i] = NULL;*/
/*}*/
