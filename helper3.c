#include "shell.h"
/**
 * process_comments - function to write comment
 * @line: number of line
 * Return: nothing
 */
void process_comments(char *line)
{
	char *comment_start = strchr(line, '#');

	if (comment_start != NULL)
	{
	*comment_start = '\0';
	}
}
/**
 * replace_pid - function to replace
 * @line: number of line
 * Return:nothing
 */
void replace_pid(char **line)
{
	char pid[10];
	char *new_line;

	sprintf(pid, "%d", getpid());

	new_line = replace_str(*line, "$$", pid);
	free(*line);
	*line = new_line;
}
/**
 * replace_exit_status - function to replace exit status
 * @line: ponter to line
 * @lastExitStatus: last line number
 * Return: nothing
 */
void replace_exit_status(char **line, int lastExitStatus)
{
	char *envValue = getenv("LAST_EXIT_STATUS");
	char *new_line;
	char exit_status[10];
	char *content;

	if (envValue == NULL)
	{
		new_line = replace_str(*line, "$?", exit_status);
		free(*line);
		*line = new_line;
		return;
	}
	content = malloc(strlen(envValue) + 1);
	if (content == NULL)
	{
		printf("Cannot allocate memory\n");
	}
	strcpy(content, envValue);
	if (content != NULL)
	{
		new_line = replace_str(*line, "$?", content);
	}
	else
	{
		sprintf(exit_status, "%d", lastExitStatus);
		new_line = replace_str(*line, "$?", exit_status);
	}
	free(*line);
	*line = new_line;
}

/**
 * replace_path - function to replace path
 * @line: pointer to char
 * Return: nothing
 */
void replace_path(char **line)
{
	char *path = getenv("PATH");

	if (path != NULL)
	{
	char *new_line = replace_str(*line, "$PATH", path);

	free(*line);
	*line = new_line;
	}
}

void process_commands(char *line, char *envp[],
		Alias aliasTable[], int *aliasCount)
{
	int num_commands, i, j;
	char **commands = split_line(line, &num_commands, ';');

	for (i = 0; i < num_commands; i++)
	{
	int num_args;
	char **args = split_line(commands[i], &num_args, ' ');

	execute_command(args, envp, aliasTable, aliasCount, num_args);

	for (j = 0; j < num_args; j++)
	{
	free(args[j]);
	}
	free(args);
	free(commands[i]);
	}
	free(commands);
}
