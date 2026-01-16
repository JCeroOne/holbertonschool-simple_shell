#ifndef _SIMPLESHELLFN_H_
#define _SIMPLESHELLFN_H_

void input(char **command, size_t *size);
void free_args(char **args);
char **parse(char *cmd);
char *getvar(char *var, char **envp);
char *cmdpath(char *cmd, char **envp);
int exec(char *name, char *cmd, char **envp);
void env(char **envp);
void error(char *name, char *cmd);

#endif /* _SIMPLESHELLFN_H_ */
