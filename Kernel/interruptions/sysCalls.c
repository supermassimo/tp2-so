#include <lib.h>
#include <time.h>
#include <console.h>
#include <standardIn.h>
#include <keyboard.h>
#include <dualDisplayManager.h>

void sysReadInput(char* buffer, size_t buffer_size);
void sysWrite(int output, const char* buffer, size_t buffer_size);
void sysWriteRegistries();
void sysWriteMemContent(char* startPos, size_t amount);
void sysWriteDateTime(int utc);
void sysSetIdle(int isIdle);
void sysClear();
int sysGetActiveDisplay();
void sysWriteCpuFeatures();
void sysSwapActiveDisplay();
void sysSleep(long seconds);

/*
void sysCallDispatcher(){
    switch((int)getRAX()){
        case 0:
            sysReadInput(getRDI(), getRSI());
            break;
        case 1:
            sysWrite(getRDI(), getRSI(), getRDX());
            break;
        case 2:
            sysWriteRegistries();
            break;
        case 3:
            sysWriteMemContent(getRDI(), getRSI());
            break;
        case 4:
            sysWriteDateTime(getRDI());
            break;
        case 5:
            sysSetIdle(getRDI());
            break;
        case 6:
            sysClear();
            break;
        case 7:
            sysGetActiveDisplay();
            break;
        case 8:
            sysWriteCpuFeatures();
            break;
        case 9:
            sysSwapActiveDisplay();
            break;
        case 10:
            sysSleep(getRDI());
            break;
        default:
            //code invalido
            break;
    }
}
*/

void sysReadInput(char* buffer, size_t buffer_size){
    getBufferContent(buffer, buffer_size);
}

void sysWrite(int output, const char* buffer, size_t buffer_size){
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

void sysWriteRegistries(){
    printRegistries();
}

void sysWriteMemContent(char* startPos, size_t amount){
    printMemContent(startPos, amount);
}

void sysWriteDateTime(int utc){
    printDateTime(utc);
}

void sysSetIdle(int isIdle){
    setIdle(isIdle);
}

void sysClear(){
    clearActiveConsole();
}

int sysGetActiveDisplay(){
    return getCurrentDisplay();
}

void sysWriteCpuFeatures(){
    printCpuFeatures();
}

void sysSwapActiveDisplay(){
    swapDisplay();
}

void sysSleep(long seconds){
    timerTickSleep(seconds);
}

void sysDelKey(){
    keyboardDeleteKey();
    deleteKeyConsole();
}