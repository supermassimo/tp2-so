#include <lib.h>
#include <console.h>
#include <standard_in.h>

void readInput(char* buffer, size_t buffer_size);
void write(int output, const char* buffer, size_t buffer_size);
void writeRegistries();

void sysCallDispatcher(){
    switch((int)getRBX()){
        case 0:
            readInput(getRDI(), getRSI());
            break;
        case 1:
            write(getRDI(), getRSI(), getRDX());
            break;
        case 2:
            writeRegistries();
            break;
        case 3:
            writeMemContent(getRDI(), getRSI());
            break;
        default:
            //code invalido
            break;
    }
}

void readInput(char* buffer, size_t buffer_size){
    getBufferContent(buffer, buffer_size);
}

void write(int output, const char* buffer, size_t buffer_size){
    switch (output)
    {
        case 0:
            print(buffer);
            break;
        case 1:
            printErr(buffer);
            break;
        default:
            break;
    }
}

void writeRegistries(){
    printRegistries();
}

void writeMemContent(char* startPos, size_t amount){
    printMemContent(startPos, amount);
}