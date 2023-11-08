#include "shell.h"

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


void execute_command(char **argv, char *envp[],Alias *aliasTable, int *aliasCount,int num_args)
{
    pid_t pid;
    int status, found;
    char *cmdpath;
    FILE* file;
    if(strcmp(argv[0], "alias") == 0) {
                if(num_args == 1) {
                    printAlias(aliasTable,aliasCount);
                    return;
                } else if(num_args == 2) {
                    char* value = getAlias(argv[1],aliasTable,aliasCount);
                    if(value != NULL) {
                        printf("%s='%s'\n", argv[1], value);
                        return;
                    }
                } else if(num_args == 3) {
                    addAlias(argv[1], argv[2],aliasTable,aliasCount);
                    return;                
                    
                }
            } 
    
    
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
        file = fopen("exit_status.txt", "w");
        if (file != NULL) {
            fprintf(file, "%d", status);
            fflush(file);
            fclose(file);
        } else {
            fclose(file);
            
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

    cmdpath = argv[0];

    found = (access(cmdpath, F_OK) == 0);

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

