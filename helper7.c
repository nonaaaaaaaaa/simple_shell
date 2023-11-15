#include "shell.h"

/**
 * prompt - Print shell prompt to stdin stream.
 *
 * Return: void.
 */
void prompt(void)
{
	_puts(PROMPT);
	fflush(stdout);
}


/**
 * handle_sigint - Signal handler for SIGINT (Ctrl+C)
 * @sig: Signal number
 *
 * Return: Nothing
 */
void handle_sigint(int sig)
{
	(void) sig;
	_putchar('\n');
	prompt();
}

/**
 * handle_sigquit - Signal handler for SIGQUIT (Ctrl+\)
 * @sig: Signal number
 *
 * Return: Nothing
 */
void handle_sigquit(int sig)
{
	(void) sig;
	_puterror("Quit (core dumped)\n");
	exit(EXIT_SUCCESS);
}

/**
 * handle_sigstp - Signal handler for SIGTSTP (Ctrl+Z)
 * @sig: Signal number
 *
 * Return: Nothing
 */
void handle_sigstp(int sig)
{
	(void) sig;
	_puts("\n");
	prompt();
}
