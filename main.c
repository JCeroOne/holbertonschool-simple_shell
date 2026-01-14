#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "simpleshellfn.h"

/**
 * error - Prints an error message
 * @name: The name used when opening the shell
 * @cmd: The command that generated the error
 */
void error(char *name, char *cmd)
{
	printf("%s: 1: %s: not found", name, cmd);
}
/**
 * env - Prints the environment variables
 * @envp: The environment to print
*/
void env(char **envp)
{
	int i = 0;

	while(envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

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

 	(void) argc;

	while(1)
	{
		if(isatty(STDIN_FILENO))
			printf("\n($) ");

		input(&cmd, &size);

		if(strcmp(cmd, "exit") == 0)
			exit(EXIT_SUCCESS);

		else if(strcmp(cmd, "env") == 0)
			env(envp);

		else if(strcmp(cmd, "errtest") == 0)
			error(argv[0], cmd);

		else
			exec(cmd, envp);
	}
}
