#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "simpleshellfn.h"

/**
 * input - Waits for and reads input from the user.
 * @cmd: A pointer to the cmd variable (char *)
 * @size: A pointer to the size variable (size_t)
*/
void input(char **cmd, size_t *size)
{
	int read_chars;

	read_chars = getline(cmd, size, stdin);

	if (read_chars == EOF)
	{
		if(isatty(STDIN_FILENO) != 0)
			printf("\n");
		
		exit(EXIT_SUCCESS);
	}

	if((*cmd)[read_chars - 1] == '\n')
		(*cmd)[read_chars - 1] = '\0';
}

/*
 * main - Entry point for the shell
 * @argc: The number of arguments passed
 * @argv: The values of the arguments
 * @envp: The environment data
 *
 * Return: 0 if succesful, an error code otherwise.
*/
int main(int argc, char *argv[], char **envp)
{
	char *cmd = NULL;
	size_t size = 0;

	printf("[DEBUG] %d %s %s\n", argc, argv[0], *envp);
	printf("\nWelcome to JC's Simple Shell!\nTo close the shell, type 'exit'\n");

	while(1)
	{
		if(isatty(STDIN_FILENO))
			printf("\n($) ");

		input(&cmd, &size);

		if(strcmp(cmd, "exit") == 0)
			exit(EXIT_SUCCESS);

		else
			exec(cmd, envp);
	}
}
