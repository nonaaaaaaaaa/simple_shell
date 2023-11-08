#include "shell.h"

void addAlias(char* name, char* value,Alias *aliasTable,int *aliasCount) {
    strcpy(aliasTable[*aliasCount].name, name);
    strcpy(aliasTable[*aliasCount].value, value);
    (*aliasCount)++;
}

char* getAlias(char* name,Alias *aliasTable,int *aliasCount) {
    	int i;
	for(i = 0; i < *aliasCount; i++) {
        	if(strcmp(aliasTable[i].name, name) == 0) {
            		return aliasTable[i].value;
        }
    }
    return NULL;
}

void printAlias(Alias *aliasTable,int *aliasCount) {
    int i;
	for(i = 0; i < *aliasCount; i++) {
        	printf("%s='%s'\n", aliasTable[i].name, aliasTable[i].value);
    }
}
