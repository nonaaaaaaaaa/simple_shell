#include "shell.h"


int main(int argc, char *envp[])
{
	char *line;
	Alias aliasTable[MAX_ALIAS_NUM];
	int aliasCount = 0;
	int lastExitStatus = 0;
	(void)argc;

	while (1)
	{
		printf("$ ");
		fflush(stdout);
		line = my_getline(STDIN_FILENO);
		if (line == NULL)
		{
			printf("\n");
			exit(EXIT_SUCCESS);
		}
		line[strlen(line) - 1] = '\0';

		process_comments(line);
		replace_pid(&line);
		replace_exit_status(&line, lastExitStatus);
		replace_path(&line);
		process_commands(line, envp, aliasTable, &aliasCount);

		free(line);
	}
	return (0);
}

