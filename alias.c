#include "shell.h"

/**
 * addAlias - add alias
 * @name: name of alias
 * @value: the value of alias
 * @aliasTable: the table pf alias
 * @aliasCount: the count
 * Return: void
 */
void addAlias(char *name, char *value, Alias *aliasTable, int *aliasCount)
{
	int i;

	for (i = 0; i < *aliasCount; i++)
	{
		if (strcmp(aliasTable[i].name, name) == 0)
		{
			strcpy(aliasTable[i].value, value);
			return;
		}
	}
	strcpy(aliasTable[*aliasCount].name, name);
	strcpy(aliasTable[*aliasCount].value, value);
	(*aliasCount)++;
}


/**
 * getAlias - get the valie of alias
 * @name: the name of alias
 * @aliasTable:the array of alias
 * @aliasCount:the count of alias
 * Return: the vlaue of alias
 */

char *getAlias(char *name, Alias *aliasTable, int *aliasCount)
{
	int i;

	for (i = 0; i < *aliasCount; i++)
	{
		if (strcmp(aliasTable[i].name, name) == 0)
		{
			return (aliasTable[i].value);
		}
	}
	return (NULL);
}

/**
 * printAlias - print the value of all alias
 * @aliasTable: array of alias
 * @aliasCount:the count of alias
 * Return: void
 */

void printAlias(Alias *aliasTable, int *aliasCount)
{
	int i;

	for (i = 0; i < *aliasCount; i++)
	{
		printf("%s='%s'\n", aliasTable[i].name, aliasTable[i].value);
	}
}
