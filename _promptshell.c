#include "shell.h"
/**
 * show_prompt-print prompt to standard output
 * Return:void
 */
void show_prompt(void)
{
	static char _original_dir[PATH_MAX] = {'\0'};
	char _cwd[PATH_MAX];

	if (_original_dir[0] == '\0')
	{
		if (getcwd(_original_dir, sizeof(_original_dir)) == NULL)
		{
			perror("getcwd");
			return;
		}
	}
	if (getcwd(_cwd, sizeof(_cwd)) != NULL)
	{
		if (strcmp(_cwd, _original_dir) == 0)
		{
			_putcharshell("Emyjakarta$ ");
		}
		else if (strstr(_cwd, _original_dir) != NULL)
		{
			_putcharshell("Emyjakarta");
			_putcharshell(_cwd + strlen(_original_dir));
			_putcharshell("$ ");
		}
		else
		{
			_putcharshell("Emyjakarta");
			_putcharshell(_cwd);
			_putcharshell("$ ");
		}
	}
	else
		perror("getcwd");
}
