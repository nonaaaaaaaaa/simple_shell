#include "shell.h"

/**
 * main - Entry point
 *
 * Return: Always 0 (Success)
 */
int main(int argc, char *envp[])
{
    char *line;
    char **commands;
    int num_commands;
    Alias aliasTable[MAX_ALIAS_NUM];
    int aliasCount = 0;
    char* comment_start;
    int lastExitStatus = 0;
    char pid[10];
    char* path;
    char exit_status[10];
    char* content;
    char* new_line;
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
        
        comment_start = strchr(line, '#');
        if(comment_start != NULL) {
            *comment_start = '\0';
        }
        
        
        
        sprintf(pid, "%d", getpid());
        new_line = replace_str(line, "$$", pid);
        free(line);
        line = new_line;

        
        content = readFileContent("exit_status.txt");
        if(content != NULL) {
            new_line = replace_str(line, "$?", content);
        }
        else{
        sprintf(exit_status, "%d", lastExitStatus);
        new_line = replace_str(line, "$?", exit_status);
        }
        free(line);
        line = new_line;

        
        path = getenv("PATH");
        if(path != NULL) {
            new_line = replace_str(line, "$PATH", path);
            free(line);
            line = new_line;
        }
        
        commands = split_line(line, &num_commands, ';');
        
        
        for (i = 0; i < num_commands; i++)
        {
            char **args;
            int num_args;
            
            args = split_line(commands[i], &num_args, ' ');
            execute_command(args, envp,aliasTable,&aliasCount,num_args);
            
            
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
