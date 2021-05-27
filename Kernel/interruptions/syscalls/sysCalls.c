#include <lib.h>
#include <console.h>
#include <standard_in.h>

static void readInput();
static void write();

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
        println("beep boop write standard out");
        break;
    case 1:
        println("beep boop write standard error");
        break;
    default:
        break;
    }
}