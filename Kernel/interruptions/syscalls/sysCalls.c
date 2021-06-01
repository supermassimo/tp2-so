#include <lib.h>
#include <console.h>
#include <standard_in.h>

void readInput();
void write();

void sysCallHandler(){
    int64_t code = getRAX();
    
    switch (code) {
    case 0:
        readInput(getRDI(), getRSI());
        break;
    case 1:
        write(getRDI(), getRSI(), getRDX());
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