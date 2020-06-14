#include "pipe.h"
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <fcntl.h>

void
pipeline(char ***cmd, char *IN, char *OUT)
{
	int fd[2];
	pid_t pid;
	int temp = 0;
    int count = 0;
    int fds;
	int status;

	while (*cmd != NULL) {
		pipe(fd);
		if ((pid = fork()) == -1) {
			perror("fork");
			exit(1);
		}
		else if (pid == 0) {
			dup2(temp, 0);
			if (*(cmd + 1) != NULL) {
				dup2(fd[1], 1);
			}

            if(count == 0 && IN != NULL){
                if((fds = open(IN, O_RDWR)) == -1){
                    printf("something?\n");
                }
                close(0);
                dup(fds);
                close(fds);
            }

            if(*(cmd + 1) == NULL && OUT != NULL){
                if((fds = open(OUT, O_RDWR|O_CREAT, 0644)) == -1){
                    printf("something?\n");
                }
                close(1);
                dup(fds);
                close(fds);                
            }

			close(fd[0]);
			execvp((*cmd)[0], *cmd);
			exit(1);
		}
		else {
			wait(&status);
			close(fd[1]);
			temp = fd[0];
			cmd++;
		}
        count++;
	}
}
