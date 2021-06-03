#include "include/mystdlib.h"

int strcmp(char* s1, char* s2){
    int c, i=-1;
    do{
        i++;
        c = s1[i] - s2[i];
    } while(s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i]);
    return c;
    // for(i=0 ; s1[i] != 0 && s2[0] != 0 ; i++){
    //     c = s1[i] - s2[i];
    //     if(c > 0)
    //         return 1;
    //     if(c < 0)
    //        return -1;
    //}
}

size_t strlen(char *string){
    size_t c=0;
    for(size_t i=0 ; string[i] != 0 ; i++){
        c++;
    }
    return c;
}

void concatStrings(char strings[][200], size_t stringAmount, char* output){
    size_t outputIdx=0, strIdx=0, i=0;
    while(strIdx < stringAmount){
        if(strings[strIdx][i] == 0){
            if(strIdx < stringAmount - 1)
                output[outputIdx] = ' ';
            else
                output[outputIdx] = 0;
            strIdx++;
            i = 0;
        }
        else
            output[outputIdx] = strings[strIdx][i];
        outputIdx++;
    }
}