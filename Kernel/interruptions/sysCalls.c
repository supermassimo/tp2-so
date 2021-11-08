// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <sysCalls.h>

void sysReadInput(int fd, char* buffer, size_t buffer_size){
    switch (fd)
    {
    case 0:
        getBufferContent(buffer, buffer_size);
        break;
    
    default:
        readPipe(fd,buffer,buffer_size);
        break;
    }
    
}

void sysWrite(int fd, const char* buffer, size_t buffer_size){
    switch (fd) {
        case 0:
            print(buffer);
            break;
        case 1:
            printErr(buffer);
            break;
        default:
            writePipe(fd,buffer,buffer_size);
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

int sysSleep(int pid, size_t seconds){
    return sleep(pid, seconds);
}

void sysDelKey(){
    keyboardDeleteKey();
}

void* sysMemAlloc(size_t size, int options){
    return memAlloc(size, options);
}

int sysMemFree(void *blockp){
    return memFree(blockp);
}

void sysGetMemInfo(MemoryInfo* meminfo){
    return getMemInfo(meminfo);
}

int sysCreateProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name){
    return createProcess(entryPoint, priority, argc, argv, name);
}

int sysNice(int pid, Priority priority){
    return nice(pid, priority);
}

int sysBlock(int pid, BlockOption option){
    return block(pid, option);
}

void sysExit(int status){
    return exit(status);
}

int sysKill(int pid, ProcessSignal sig){
    return kill(pid, sig);
}

void sysPrintAllProcesses(){
    return printAllProcesses();
}

int sysGetpid(){
    return getpid();
}

void sysSkip(){
    return skip();
}

int sysSemOpen(char* sem_id, int value){
    return sem_init(sem_id, value);
}

int sysSemDestroy(char* sem_id){
    return sem_destroy(sem_id);
}

int sysSemWait(char* sem_id){
    return sem_wait(sem_id);
}

int sysSemPost(char* sem_id){
    return sem_post(sem_id);
}

int sysSemSetValue(char* sem_id, int value){
    return sem_set_value(sem_id, value);
}

void sysPrintAllSemaphores() {
    return printAllSemaphores();
}

int sysCreatePipe() {
    return createPipe();
}

void sysClosePipe(int index) {
    return closePipe(index);
}

int sysWritePipe(int index, const char* buf, int count) {
    return writePipe(index,buf,count);
}

int sysReadPipe(int index, char* buf, int count) {
    return readPipe(index,buf,count);
}

void sysPrintPipes() {
    return printPipes();
}

void *sysSharedMemGet(int id) {
    return get(id);
}