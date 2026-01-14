#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "simpleshellfn.h"

void free_args(char **args)
{
	while(*args)
	{
		free(*args);
		args++;
	}
	free(args);
}

char **parse(char *cmd)
{
	char *copy = strdup(cmd);
	char *tok = NULL;
	char **args = NULL;
	int i = 0;
	int argc = 0;

	if(!copy)
		return (NULL);

	tok = strtok(copy, " ");

	while(tok)
	{
		argc++;
		tok = strtok(NULL, " ");
	}

	free(copy);
	args = malloc((argc + 1) * sizeof(char *));

	if(!args)
		return (NULL);
	
	copy = strdup(cmd);

	if(!copy)
	{
		free_args(args);
		return (NULL);
	}

	tok = strtok(copy, " ");

	for (i = 0; tok; i++)
	{
		args[i] = strdup(tok);

		if(!args[i])
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

int exec(char *cmd, char **envp)
{
	char **args = parse(cmd);
	printf("%s, %s, %s (%s)", args[0], args[1], args[2], *envp);
	free(args);
	return (1);
}
