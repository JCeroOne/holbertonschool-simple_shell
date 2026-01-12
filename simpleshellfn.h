#ifndef _SIMPLESHELLFN_H_
#define _SIMPLESHELLFN_H_

void input(char **command, size_t size);
void parse(char command[], char **envp);
int exec(char *command[], char **envp);
char **get_path(char **cmd, char **envp);

#endif /* _SIMPLESHELLFN_H_ */
