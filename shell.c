#include "shell.h"

int main(int argc, char *envp[])
{
    	
    char *line;
    char **commands;
    int num_commands;
    (void)argc;
    while (1)
    {
        int i,j;
        printf("$ ");
        fflush(stdout);
        line = my_getline(STDIN_FILENO);
         if (line == NULL)
        {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        line[strlen(line) - 1] = '\0';
        
        commands = split_line(line, &num_commands, ';');
        
        
        for (i = 0; i < num_commands; i++)
        {
            char **args;
            int num_args;
            
            args = split_line(commands[i], &num_args, ' ');
            execute_command(args, envp);
            
            
            for (j=0; j < num_args; j++)
            {
                free(args[j]);
            }
            free(args);

            free(commands[i]);
        }

        free(commands);
        free(line);
    }

    return 0;
}
