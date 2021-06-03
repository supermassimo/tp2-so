#include <stddef.h>
#include <stdint.h>

size_t numToStr(size_t value, char* target, size_t base);
size_t strToNumPos(char* string);
int strcmp(char* s1, char* s2);
size_t strlen(char *string);
size_t concatStrings(char** strings, size_t stringAmount, char* output);