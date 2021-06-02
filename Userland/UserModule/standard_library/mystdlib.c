#include "include/mystdlib.h"

int strcmp(char* s1, char* s2){
    int c;
    for(int i=0 ; s1[i] != 0 && s2[0] != 0 ; i++){
        c = s1[i] - s2 [i];
    }
    return c;
}

int strlen(char *string){
    int c;
    for(int i=0 ; string[i] != 0 ; i++){
        c++;
    }
    return c;
}