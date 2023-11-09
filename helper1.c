#include "shell.h"

/**
 * cd_command - handle cd
 * @path: path to cd
 * Return: void
 */
void cd_command(char *path)
{
	char *home = getenv("HOME");
	char *prev = getenv("OLDPWD");
	char *current = getenv("PWD");
	char new_current[1024];

	if (path == NULL || strcmp(path, "~") == 0)
	{
		path = home;
	}
	else if (strcmp(path, "-") == 0)
	{
		if (prev == NULL)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return;
		}
		path = prev;
		printf("%s\n", path);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return;
	}
	setenv("OLDPWD", current, 1);
	getcwd(new_current, sizeof(new_current));
	setenv("PWD", new_current, 1);
	}

/**
 * handle_setenv - handle setenv
 * @argv:arguments
 * Return:void
 */
void handle_setenv(char **argv)
{
	if (argv[1] == NULL || argv[2] == NULL)
	{
		fprintf(stderr, "setenv: Missing arguments\n");
		return;
	}
	if (setenv(argv[1], argv[2], 1) == -1)
	{
		perror("setenv");
	}
}

/**
 * handle_unsetenv - handle ensetenv
 * @argv: arguments
 * Return:void
 */
void handle_unsetenv(char **argv)
{
	if (argv[1] == NULL)
	{
		fprintf(stderr, "unsetenv: Missing argument\n");
		return;
	}
	if (unsetenv(argv[1]) == -1)
	{
		perror("unsetenv");
	}
}

/**
 * replace_str - to replace string
 * @str:strng
 * @old_sub:old string
 * @new_sub:new
 * Return: new string
 */
char *replace_str(char *str, char *old_sub, char *new_sub)
{
	char *result;
	int i, count = 0;
	int new_sub_len = strlen(new_sub);
	int old_sub_len = strlen(old_sub);

	for (i = 0; str[i] != '\0'; i++)
	{
		if (strstr(&str[i], old_sub) == &str[i])
		{
			count++;
			i += old_sub_len - 1;
		}
	}
	result = (char *)malloc(i + count * (new_sub_len - old_sub_len) + 1);
	i = 0;
	while (*str)
	{
		if (strstr(str, old_sub) == str)
		{
			strcpy(&result[i], new_sub);
			i += new_sub_len;
			str += old_sub_len;
		}
		else
		{
			result[i++] = *str++;
		}
	}
	result[i] = '\0';
	return (result);
}
