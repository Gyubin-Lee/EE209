#include <stdio.h>
#include <stdlib.h>

int main(){
    int *arr_p = (int*)malloc(sizeof(int)*3);

    for(int i=0; i<3;i++){
        arr_p[i] = i;
        printf("%d\n", arr_p[i]);
    }

    int *old = arr_p;
    arr_p = realloc(old, sizeof(int)*6);

    for(int j=3; j<6;j++){
        arr_p[j] = j;
    }

    for(int k=0;k<6;k++){
        printf("%d\n", arr_p[k]);
    }

}