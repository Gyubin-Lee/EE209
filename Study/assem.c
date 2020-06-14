#include <stdio.h>

int add2(int x, int y){
    int d = x+y;
    return d;
}

int add3(int x, int y, int z){
    int d = x+y+z;
    return add2(d, 4);
}

int main(){
    int x = add3(1, 2, 3);
    return 0;
}