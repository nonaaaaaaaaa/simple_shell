#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 1024
#define MAXLINE 1024
#define MAXARGS 128


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




char *my_getline(int fd)
{
    static char buffer[BUFFER_SIZE];
    static char *ptr = buffer;
    static int bytes_left = 0;
    char *line, *newline;
    int line_len;

    if (bytes_left == 0)
    {
        bytes_left = read(fd, buffer, BUFFER_SIZE);
        if (bytes_left == -1)
            return NULL; 
        else if (bytes_left == 0)
            
            return NULL;
        ptr = buffer;
    }

    newline = memchr(ptr, '\n', bytes_left);
    if (newline == NULL)
        newline = ptr + bytes_left - 1;
    line_len = newline - ptr + 1;

    line = malloc(line_len + 1);
    if (line == NULL)
        return NULL; 

    memcpy(line, ptr, line_len);
    line[line_len] = '\0';

    ptr += line_len;
    bytes_left -= line_len;

    return line;
}

char **split_line(char *line, int *argc, char separator)
{
    char **argv = malloc(MAXARGS * sizeof(char *));
    char *start = line;
    char *end;
    int i = 0;

    while (1)
    {
        while (*start == separator) start++;  
        if (*start == '\0') break;  

        end = start;
        while (*end != separator && *end != '\0') end++;

        argv[i] = malloc((end - start + 1) * sizeof(char));
        strncpy(argv[i], start, end - start);
        argv[i][end - start] = '\0';

        i++;
        start = end;
    }

    argv[i] = NULL;
    *argc = i;

    return argv;
}


void execute_command(char **argv, char *envp[])
{
    pid_t pid;
    int status;
    char *cmdpath;

    if (strcmp(argv[0], "cd") == 0)
    {
        cd_command(argv[1]);
        return;
    }

    if (strcmp(argv[0], "exit") == 0)
    {
        int status = 0;
        if (argv[1] != NULL)
        {
            status = atoi(argv[1]);
            if (status < 0)
            {
                fprintf(stderr, "exit: Illegal number: %s\n", argv[1]);
                return;
            }
        }
        exit(status);
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
        char **env;
        for (env = envp; *env != 0; env++)
        {
            char* thisEnv = *env;
            printf("%s\n", thisEnv);    
        }
        return;
    }

    *cmdpath = argv[0];

    int found = (access(cmdpath, F_OK) == 0);

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
                cmdpath = tmp;
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
        return;
    }

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
 * main - Entry point
 *
 * Return: Always 0 (Success)
 */
int main(int argc, char *argv[], char *envp[])
{
    char *line;
    char **commands;
    int num_commands;

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
