#include "shell.h"

void process_comments(char* line) {
    char* comment_start = strchr(line, '#');
    if (comment_start != NULL) {
        *comment_start = '\0';
    }
}

void replace_pid(char** line) {
    char pid[10];
    char * new_line;
    sprintf(pid, "%d", getpid());

    new_line = replace_str(*line, "$$", pid);
    free(*line);
    *line = new_line;
}

void replace_exit_status(char** line, int lastExitStatus) {
    char* content = readFileContent("exit_status.txt");
    char* new_line;
    if (content != NULL) {
        new_line = replace_str(*line, "$?", content);
    } else {
        char exit_status[10];
        sprintf(exit_status, "%d", lastExitStatus);
        new_line = replace_str(*line, "$?", exit_status);
    }
    free(*line);
    *line = new_line;
}
void replace_path(char** line) {
    char* path = getenv("PATH");
    if (path != NULL) {
        char* new_line = replace_str(*line, "$PATH", path);
        free(*line);
        *line = new_line;
    }
}

void process_commands(char* line, char *envp[], Alias aliasTable[], int *aliasCount) {
    int num_commands, i, j;
    char** commands = split_line(line, &num_commands, ';');
    for (i = 0; i < num_commands; i++) {
        int num_args;
        char** args = split_line(commands[i], &num_args, ' ');
        execute_command(args, envp, aliasTable, aliasCount, num_args);
        for (j = 0; j < num_args; j++) {
            free(args[j]);
        }
        free(args);
        free(commands[i]);
    }
    free(commands);
}
