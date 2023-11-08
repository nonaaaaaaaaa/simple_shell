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

void cd_command(char *path);
void handle_setenv(char **argv);
void handle_unsetenv(char **argv);
char *my_getline(int fd);
char **split_line(char *line, int *argc, char separator);
void execute_command(char **argv, char *envp[]);
#endif
