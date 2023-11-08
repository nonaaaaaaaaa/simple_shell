#include "shell.h"


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

