#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "simpleshellfn.h"

/**
 * free_args - Frees the argument variable
 * @args: The argument variable to free
 */
void free_args(char **args)
{
	while (*args)
	{
		free(*args);
		args++;
	}
	free(args);
}

/**
 * getvar - Gets the contents of a variable within the environment
 * @var: The variable to get
 * @envp: The environment to search in.
 *
 * Return: A pointer to the contents of the variable.
 */
char *getvar(char *var, char **envp)
{
	char **env;
	char *val = NULL;
	int eqPos;

	for(env = envp; *env != NULL; env++)
	{
		eqPos = 0;
		while ((*env)[eqPos])
		{
			if((*env)[eqPos] == '=')
				break;
			eqPos++;
		}

		if((*env)[eqPos] != '=')
			continue;

		if(strncmp(*env, var, (size_t) eqPos) != 0)
			continue;

		val = strdup(*env + eqPos + 1);
		return (val);
	}

	return (NULL);
}

/**
 * cmdallowed - Checks if the given command exists and is accesible.
 * @cmd: The command to check
 * @path: The path to search within
 * @envp: The local environment
 *
 * Return: 1 if the command exists and is accesible, 0 otherwise.
 */
int cmdallowed(char **cmd, char *path, char **envp)
{
	printf("%s, %s, %s", *cmd, path, *envp);
	return (0);
}

/**
 * parse - Extracts the arguments from the given command string
 * @cmd: The command string to parse
 *
 * Return: A pointer to the arguments
 */
char **parse(char *cmd)
{
	char *copy = strdup(cmd);
	char *tok = NULL;
	char **args = NULL;
	int i = 0;
	int argc = 0;

	if (!copy)
		return (NULL);
	tok = strtok(copy, " ");
	while (tok)
	{
		argc++;
		tok = strtok(NULL, " ");
	}
	free(copy);
	args = malloc((argc + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	copy = strdup(cmd);
	if (!copy)
	{
		free_args(args);
		return (NULL);
	}
	tok = strtok(copy, " ");
	for (i = 0; tok; i++)
	{
		args[i] = strdup(tok);
		if (!args[i])
		{
			free_args(args);
			free(copy);
			return (NULL);
		}
		tok = strtok(NULL, " ");
	}
	args[i] = NULL;
	free(copy);
	return (args);
}

/**
 * exec - Runs a command within the local environment
 * @cmd: The command string to run
 * @envp: The local environment
 *
 * Return: 1 if succesful
 */
int exec(char *cmd, char **envp)
{
	char **args = parse(cmd);
	char *env = getvar("PATH", envp);

	printf("%s", env);
	free(env);

	free(args);
	return (1);
}
