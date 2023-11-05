#ifndef SHELL_H
#define SHELL_H
#define GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORM 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - single linked list
 * @num: field number
 * @str: string
 * @next: point to next node
 */

typedef struct liststr
{
	int num;
	char *str;
	struct listrstr *next;
} list_t;

/**
 * struct passinfo - contain pseudo arguments
 * @arg: string argument
 * @argv:array of string
 * @path:string path for current command
 * @argc: argument count
 * @line_count: error count
 * @err_num: the error code for exit
 * @linecount_flag: if on count this line of input
 * @fname: the filename program
 * @env: linked list local copy of environ
 * @history: the history node
 * @alias: the alias node
 * @environ:custom modifiefd copy
 * @env_changed: on if environ was changed
 * @status: the return status
 * @cmd_buf: address of pointer
 * @cmd_buf_type: cmd_type || , && , ;
 * @readfd: to rad line input
 * @histcount: line num count history
 */

typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;
	char **cmd_buf;
	int cmd_buf_type;
	int readfd;
	int histcount;
} info_t;
