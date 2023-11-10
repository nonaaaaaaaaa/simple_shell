#include "shell.h"

/**
 * handle_alias - handle alias
 * @argv: argument
 * @num_args: num of args
 * @aliasTable: array of alias
 * @aliasCount: num of alias
 * Return:void
 */
void handle_alias(char **argv, int num_args,
		Alias *aliasTable, int *aliasCount)
{
	if (num_args == 1)
	{
		printAlias(aliasTable, aliasCount);
	}
	else if (num_args == 2)
	{
		char *value = getAlias(argv[1], aliasTable, aliasCount);

		if (value != NULL)
		{
			printf("%s='%s'\n", argv[1], value);
		}
	}
	else if (num_args == 3)
	{
		addAlias(argv[1], argv[2], aliasTable, aliasCount);
	}
}

/**
 * handle_exit - handle exit
 * @argv: arguments
 * Return:void
 */
void handle_exit(char **argv)
{
	int status = 0;
	char status_str[20];

	if (argv[1] != NULL)
	{
		status = atoi(argv[1]);
		if (status < 0)
		{
			fprintf(stderr, "exit: Illegal number: %s\n", argv[1]);
			return;
		}
	}
	sprintf(status_str, "%d", status);
	setenv("LAST_EXIT_STATUS", status_str, 1);
	exit(status);
}

/**
 * handle_env - handle env
 * @envp:environment
 * Return:void
 */
void handle_env(char *envp[])
{
	char **env;

	for (env = envp; *env != 0; env++)
	{
		char *thisEnv = *env;

		printf("%s\n", thisEnv);
	}
}

/**
 * handle_command - handle commands
 * @argv: arguments
 * @cmdpath: path
 * Return:void
 */
void handle_command(char **argv, char *cmdpath)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("Error:");
		return;
	}
	if (pid == 0)
	{
		if (execv(cmdpath, argv) == -1)
		{
		perror("Error:");
		}
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&status);
	}
}

/**
 * find_command_path - find path
 * @cmdpath: path of cmd
 * @argv: aguments
 * Return:void
 */
void find_command_path(char **cmdpath, char **argv)
{
	int found = (access(*cmdpath, F_OK) == 0);

	if (!found && strchr(argv[0], '/') == NULL)
	{
		char *path = strdup(getenv("PATH"));
		char *p = strtok(path, ":");
		char tmp[512];

		while (p != NULL)
		{
			snprintf(tmp, sizeof(tmp), "%s/%s", p, argv[0]);
			if (access(tmp, F_OK) == 0)
			{
				*cmdpath = tmp;
				found = 1;
				break;
			}
			p = strtok(NULL, ":");
		}
		free(path);
	}
	if (!found)
	{
		fprintf(stderr, "%s: command not found\n", argv[0]);
	}
}
