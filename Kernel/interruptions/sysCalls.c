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

void sysGetDateTime(Date* date, Time* time, int utc){
    getDateChanged(date, getTime(time, utc));
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

bool sysGetCpuFeatures(CommonFeatures* commonFeatures, ExtendedFeatures* extendedFeatures){
    return getCpuFeatures(commonFeatures, extendedFeatures);
}

void sysSwapActiveDisplay(){
    swapDisplay();
}

void sysSleep(size_t seconds){
    timerTickSleep(seconds);
}

void sysDelKey(){
    keyboardDeleteKey();
}

void sysSetupConsole(const ConsoleParameters consoleValues){

}