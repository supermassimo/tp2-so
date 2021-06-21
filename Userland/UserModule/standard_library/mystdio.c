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

static void printRegistry(const char* msg, uint64_t value){
	printf(msg);
	printInt(value, 16, 16);
	printf("\n");
}

void printRegistries(const Registries regs){
	printRegistry("RAX: ", regs.RAX);
	printRegistry("RBX: ", regs.RBX);
	printRegistry("RCX: ", regs.RCX);
	printRegistry("RDX: ", regs.RDX);
	printRegistry("RDI: ", regs.RDI);
	printRegistry("RSI: ", regs.RSI);
	printRegistry("R8: " , regs.R8 );
	printRegistry("R9: " , regs.R9 );
	printRegistry("R10: ", regs.R10);
	printRegistry("R11: ", regs.R11);
	printRegistry("R12: ", regs.R12);
	printRegistry("R13: ", regs.R13);
	printRegistry("R14: ", regs.R14);
	printRegistry("R15: ", regs.R15);
	printRegistry("RBP: ", regs.RBP);
	printRegistry("RSP: ", regs.RSP);
	printRegistry("RIP: ", regs.RIP);
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