#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(){
    int pid, p[2], status;

    if (pipe(p) == -1){
        exit(1);
    }
    
    pid = fork();
    if(pid == 0) {
        close(p[1]);
        dup2(p[0], 0);
        close(p[0]);

        execvp("echo", "echo");
    } else{
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);
        printf("hello world\n");

        wait(&status);
    }
}