#include <lib.h>
#include <time.h>
#include <console.h>
#include <standardIn.h>
#include <keyboard.h>
#include <dualDisplayManager.h>
#include <sysCalls.h>

void sysReadInput(char* buffer, size_t buffer_size){
    getBufferContent(buffer, buffer_size);
}

void sysWrite(int output, const char* buffer, size_t buffer_size){
    switch (output) {
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

void sysGetMemContent(uint64_t startPos, uint8_t* target, size_t amount){
    getMemContent(startPos, target, amount);
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
}