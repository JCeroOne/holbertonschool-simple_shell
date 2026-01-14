#ifndef _SIMPLESHELLFN_H_
#define _SIMPLESHELLFN_H_

void input(char **command, size_t *size);
void free_args(char **args);
char **parse(char *cmd);
int exec(char *cmd, char **envp);
char **get_path(char **cmd, char **envp);

#endif /* _SIMPLESHELLFN_H_ */
