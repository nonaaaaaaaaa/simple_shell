#include "shell.h"

/**
 * readFileContent - read the last exit statue from file
 * @fileName: filename
 * Return:contant
 */

char *readFileContent(char *fileName)
{
	FILE *file;
	char *content;
	size_t n = 0;
	int c;

	file = fopen(fileName, "r");
	if (file == NULL)
	{
	fclose(file);
	return (NULL);
	}
	content = malloc(1000);
	if (content == NULL)
	{
		printf("Cannot allocate memory \n");
		return (NULL);
	}
	while ((c = fgetc(file)) != EOF)
	{
		content[n++] = (char)c;
	}
	content[n] = '\0';
	content[strcspn(content, "\n")] = '\0';
	fclose(file);
	return (content);
}
