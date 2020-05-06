#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    char **p = calloc(1000, sizeof(char*));

    p[3] = "aaaa";
    printf("pointer p:%p\n", p);
    printf("pointer p[3]:%p\n", p[3]);
    printf("content p[3]:%s\n", p[3]);

    char *q = p[3];
    free(q);

    printf("pointer p:%p\n", p);
    printf("pointer p[3]:%p\n", p[3]);
    printf("content p[3]:%s\n", p[3]);

}