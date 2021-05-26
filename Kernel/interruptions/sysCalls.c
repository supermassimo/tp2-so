#include <lib.h>
#include <console.h>

void sysCallHandler(){
    int64_t code = getRAX();
    
    switch (code) {
    case 0:
        read(getRDI(), getRSI(), getRDX());
        break;
    case 1:
        write(getRDI(), getRSI(), getRDX());
        break;
    
    default:
        
        break;
    }

}

void read(int file_descriptor, char* buffer, size_t buffer_size){
    println("beep boop read");
}

void write(int file_descriptor, const char* buffer, size_t buffer_size){
    println("beep boop write");
}