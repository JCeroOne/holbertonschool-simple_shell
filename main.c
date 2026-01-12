#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "simpleshellfn.h"

void input(char **cmd, size_t *size)
{
	int read_chars;

	read_chars = getline(cmd, size, stdin);

	if (read_chars == -1)
	{
		if(isatty(STDIN_FILENO) != 0)
			printf("\n");
		
		exit(EXIT_SUCCESS);
	}

	if((*cmd)[read_chars - 1] == '\n')
		(*cmd)[read_chars - 1] = '\0';
}

int main(int argc, char *argv[], char **envp)
{
	char *cmd = NULL;
	size_t size = 0;

	printf("%d %s %s", argc, argv[0], *envp);

	while(1)
	{
		if(isatty(STDIN_FILENO))
			printf("($) ");

		input(&cmd, &size);

		if(strcmp(cmd, "exit") == 0)
			exit(EXIT_SUCCESS);

		else
			printf("Nein");
	}
}
