#include "shell.h"
/**
 * my_getline - function to get new line
 * @fd: int number
 * Return: Null
 */
char *my_getline(int fd)
{
	static char buffer[BUFFER_SIZE];
	static char *ptr = buffer;
	static int bytes_left;
	char *line, *newline;
	int line_len;

	if (bytes_left == 0)
	{
		bytes_left = read(fd, buffer, BUFFER_SIZE);
		if (bytes_left == -1)
			return (NULL);
		else if (bytes_left == 0)
			return (NULL);
		ptr = buffer;
	}
	newline = memchr(ptr, '\n', bytes_left);
	if (newline == NULL)
		newline = ptr + bytes_left - 1;
	line_len = newline - ptr + 1;
	line = malloc(line_len + 1);
	if (line == NULL)
		return (NULL);
	memcpy(line, ptr, line_len);
	line[line_len] = '\0';
	ptr += line_len;
	bytes_left -= line_len;
	return (line);
}
/**
 * split_line - function to split line
 * @line: pointer
 * @argc: pointer
 * @separator: character variable
 * Return: argv
 */
char **split_line(char *line, int *argc, char separator)
{
	char **argv = malloc(MAXARGS * sizeof(char *));
	char *start = line;
	char *end;
	int i = 0;

	while (1)
	{
		while (*start == separator)
			start++;
		if (*start == '\0')
		break;
		end = start;
		while (*end != separator && *end != '\0')
			end++;
		argv[i] = malloc((end - start + 1) * sizeof(char));
		strncpy(argv[i], start, end - start);
		argv[i][end - start] = '\0';
		i++;
		start = end;
	}
	argv[i] = NULL;
	*argc = i;
	return (argv);
}


void execute_command(char **argv, char *envp[], Alias *aliasTable,
		int *aliasCount, int num_args)
{
	char *cmdpath = argv[0];

	if (strcmp(argv[0], "alias") == 0)
	{
		handle_alias(argv, num_args, aliasTable, aliasCount);
		return;
	}
	if (strcmp(argv[0], "cd") == 0)
	{
		cd_command(argv[1]);
		return;
	}
	if (strcmp(argv[0], "exit") == 0)
	{
		handle_exit(argv);
		return;
	}
	if (strcmp(argv[0], "setenv") == 0)
	{
		handle_setenv(argv);
		return;
	}
	if (strcmp(argv[0], "unsetenv") == 0)
	{
		handle_unsetenv(argv);
		return;
	}
	if (strcmp(argv[0], "env") == 0)
	{
		handle_env(envp);
		return;
	}
	find_command_path(&cmdpath, argv);
	handle_command(argv, cmdpath);
}
