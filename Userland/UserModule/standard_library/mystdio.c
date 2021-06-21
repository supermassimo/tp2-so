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

void printRegistries(uint64_t *regs){
	printf("RAX: ");
	printInt(regs[0], 10, 10);
	printf("\n");
	printf("RBX: ");
	printInt(regs[1], 10, 10);
	printf("\n");
	printf("RCX: ");
	printInt(regs[2], 10, 10);
	printf("\n");
	printf("RDX: ");
	printInt(regs[3], 10, 10);
	printf("\n");
	printf("RBP: ");
	printInt(regs[4], 10, 10);
	printf("\n");
	printf("RDI: ");
	printInt(regs[5], 10, 10);
	printf("\n");
	printf("RSI: ");
	printInt(regs[6], 10, 10);
	printf("\n");
	printf("R8: ");
	printInt(regs[7], 10, 10);
	printf("\n");
	printf("R9: ");
	printInt(regs[8], 10, 10);
	printf("\n");
	printf("R10: ");
	printInt(regs[9], 10, 10);
	printf("\n");
	printf("R11: ");
	printInt(regs[10], 10, 10);
	printf("\n");
	printf("R12: ");
	printInt(regs[11], 10, 10);
	printf("\n");
	printf("R13: ");
	printInt(regs[12], 10, 10);
	printf("\n");
	printf("R14: ");
	printInt(regs[13], 10, 10);
	printf("\n");
	printf("R15: ");
	printInt(regs[14], 10, 10);
	printf("\n");
	printf("RSP: ");
	printInt(regs[15], 10, 10);
	printf("\n");
	printf("RIP: ");
	printInt(regs[16], 10, 10);
	printf("\n");
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