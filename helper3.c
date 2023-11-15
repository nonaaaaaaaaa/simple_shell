#include "shell.h"

/**
 * _puts - Prints a string to the standard output stream
 * @str: The string to print
 *
 * Return: Void
 */
void _puts(char *str)
{
	size_t len;
	ssize_t num_written;

	len = _strlen(str);
	num_written = write(STDOUT_FILENO, str, len);
	if (num_written == -1)
	{
		perror("write");
	}
}

/**
 * _puterror - Prints an error message to the standard error stream
 * @err: The error message to print
 *
 * Return: Void
 */
void _puterror(char *err)
{
	size_t len;
	ssize_t num_written;

	len = _strlen(err);
	num_written = write(STDERR_FILENO, err, len);
	if (num_written == -1)
	{
		perror("write");
	}
}

/**
 * find_in_path - Looks for a command in each directory specified in the PATH
 *                environment variable
 * @command: pointer to `command` string to look for.
 *
 * Return: pointer to string containing the full path (success) if it is found,
 *         or NULL if it is not found (failure).
*/
char *find_in_path(char *command)
{
	struct stat st;
	int stat_ret, i;
	char buf[PATH_MAX_LENGTH], *path, *ret, **dir;

	path = get_path();
	if (!path)
		return (NULL);
	dir = tokenize(path, PATH_SEPARATOR);
	if (!dir)
		return (NULL);
	for (i = 0; dir[i]; i++)
	{
		_memset(buf, 0, PATH_MAX_LENGTH);
		_strcpy(buf, dir[i]);
		_strcat(buf, "/");
		_strcat(buf, command);
		stat_ret = stat(buf, &st);
		if (stat_ret == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
		{
			free_tokens(dir);
			ret = malloc(sizeof(char) * (strlen(buf) + 1));
			if (!ret)
				return (NULL);
			strcpy(ret, buf);
			return (ret);
		}
	}
	if (stat_ret == -1)
		free_tokens(dir);
	return (NULL);
}

/**
 * _getenv - Get the value of an environment variable
 * @name: Name of the environment variable
 *
 * Return: Value of the environment variable, or NULL if it doesn't exist
 */
char *_getenv(const char *name)
{
	char **env;
	size_t name_len = _strlen(name);

	for (env = environ; *env != NULL; env++)
	{
		if (_strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
		{
			return (&(*env)[name_len + 1]);
		}
	}

	return (NULL);
}


