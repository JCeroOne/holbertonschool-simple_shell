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
	printf("%s: 1: %s: not found\n", name, cmd);
}
/**
 * env - Prints the environment variables
 * @envp: The environment to print
*/
void env(char **envp)
{
	int i = 0;

	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

/**
 * trim - Removes all spaces surrounding the command
 * @cmd: The command to trim
 */
void trim(char *cmd)
{
	int start = 0;
	int end = 0;

	while (cmd[start] == ' ' || cmd[start] == '\t')
		start++;

	if (start > 0)
		memmove(cmd, cmd + start, strlen(cmd + start) + 1);

	end = strlen(cmd) - 1;

	while (end >= 0 && (cmd[end] == ' ' || cmd[end] == '\t'))
	{
		cmd[end] = '\0';
		end--;
	}
}

/**
 * input - Waits for and reads input from the user.
 * @cmd: A pointer to the cmd variable (char *)
 * @size: A pointer to the size variable (size_t)
 *
 * Return: 1 if the input is valid, 0 if its only spaces
*/
int input(char **cmd, size_t *size)
{
	int read_chars;

	read_chars = getline(cmd, size, stdin);

	if (read_chars == EOF)
	{
		if (isatty(STDIN_FILENO) != 0)
			printf("\n");
		
		free(*cmd);
		exit(EXIT_SUCCESS);
	}

	if ((*cmd)[read_chars - 1] == '\n')
		(*cmd)[read_chars - 1] = '\0';

	trim(*cmd);

	if(strlen(*cmd) > 0)
		return (1);

	return (0);
}

/**
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
	int valid = 0;
	int status = 0;
	(void) argc;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("($) ");
		
		valid = input(&cmd, &size);

		if(valid == 0)
			continue;
		
		if (strncmp(cmd, "exit", 4) == 0)
		{
			free(cmd);
			exit(status);
		}

		else if (strncmp(cmd, "env", 3) == 0)
		{
			env(envp);
			status = 0;
		}

		else
			status = exec(argv[0], cmd, envp);
	}
}
