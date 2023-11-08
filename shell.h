#ifndef SHELL_H
#define SHELL_H
#define GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 1024
#define MAXLINE 1024
#define MAXARGS 128
#define MAX_ALIAS_NUM 100
#define MAX_ALIAS_NAME 100
#define MAX_ALIAS_VALUE 100

typedef struct {
    char name[MAX_ALIAS_NAME];
    char value[MAX_ALIAS_VALUE];
} Alias;


void cd_command(char *path);
void handle_setenv(char **argv);
void handle_unsetenv(char **argv);
char *my_getline(int fd);
char **split_line(char *line, int *argc, char separator);
void execute_command(char **argv, char *envp[],Alias *aliasTable, int *aliasCount,int num_args);
void addAlias(char* name, char* value,Alias *aliasTable,int *aliasCount);
char* getAlias(char* name,Alias *aliasTable,int *aliasCount);
void printAlias(Alias *aliasTable,int *aliasCount);
char* replace_str(char* str, char* old_sub, char* new_sub);
char* readFileContent(char *fileName);
#endif
