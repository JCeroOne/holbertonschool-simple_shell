#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "simpleshellfn.h"

/**
 * free_args - Frees the argument variable
 * @args: The argument variable to free
 */
void free_args(char **args)
{
	int i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
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
 * cmdpath - Returns the path of the given command.
 * @cmd: The command to check
 * @envp: The local environment
 *
 * Return: The path of the file to run. NULL if the command isn't accesible.
 */
char *cmdpath(char *cmd, char **envp)
{
	char *paths = NULL;
	char *copy = NULL;
	char *path;
	char *tok;
	
	if (strchr(cmd, '/'))
	{
		if(access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	paths = getvar("PATH", envp);
	if(!paths)
		return (NULL);
	if(paths[0] == '\0')
	{
		free(paths);
		return (NULL);
	}
	copy = strdup(paths);
	if(!copy)
	{
		free(paths);
		return (0);
	}
	tok = strtok(copy, ":");
	while (tok)
	{
		path = malloc(strlen(tok) + strlen(cmd) + 2);	
		if(cmd[0] != '/' && strncmp(cmd, "./", 2) != 0)
			sprintf(path, "%s/%s", tok, cmd);
		else
			sprintf(path, "%s", cmd);
		if(access(path, X_OK) == 0)
		{
			free(copy);
			free(paths);
			return (path);
		}	
		free(path);
		tok = strtok(NULL, ":");
	}
	free(copy);
	free(paths);
	return (NULL);
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
 * @name: The name used to run the shell
 * @cmd: The command string to run
 * @envp: The local environment
 *
 * Return: 1 if succesful
 */
int exec(char *name, char *cmd, char **envp)
{
	pid_t id;
	int status;
	char **args = parse(cmd);
	char *path = cmdpath(args[0], envp);

	if (!path)
	{
		error(name, args[0]);
		free_args(args);
		return (127);
	}

	id = fork();

	if (id < 0)
	{
		printf("%s: 1: %s: fork failed", name, args[0]);
		free(path);
		free_args(args);
		return (1);
	}

	if (id > 0)
	{
		waitpid(id, &status, 0);
		free(path);
		free_args(args);
		
		if(WIFEXITED(status))
			return (WEXITSTATUS(status));

		return (1);
	}

	execve(path, args, envp);

	perror(name);
	free(path);
	free_args(args);
	exit(126);
}
