#include "./include/mystdio.h"
#include "./include/mystdlib.h"

#define STD_OUT 0
#define STD_ERR 1

typedef unsigned char uint8_t;

extern void write(int output, const char* buffer, size_t buffer_size);
extern void readInput(char* buffer, size_t buffer_size);
extern void swapConsole();
extern void delKey();

void printErr(const char* string){
	write(STD_ERR, string, strlen(string));
}

void printf(const char* string){
    write(STD_OUT, string, strlen(string));
}

void printInt (long num, size_t string_size, uint8_t base){
    char array[string_size];
    int final_size = numToStr(num, array, base);
    char final_string[final_size+1];
    for (int i=0; i<=final_size; i++){
        final_string[i] = array[i];
    }
    write (0, final_string, string_size);
}

void printFloat (float num, size_t string_size, size_t precision, uint8_t base){
    char array[string_size];
    int final_size = floatToStr(num, array, precision, base);
    char final_string[final_size+1];
    for (int i=0; i<=final_size; i++){
        final_string[i] = array[i];
    }
    write (0, final_string, string_size);
}

void scanf (char* string, size_t string_size){
    readInput (string, string_size);
}

void consoleSwap(){
	swapConsole();
}

void deleteKey(){
	delKey();
}