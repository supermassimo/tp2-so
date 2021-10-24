#include <scheduler.h>

#define MAX_PROCESSES  10
#define PCB_REGISTERS  21
#define PROCESS_STACK  400

typedef enum State {TERMINATED, READY, BLOCKED, SUSPENDED} State;

typedef struct {
    char* name;
    State state;
    Priority priority;
    uint64_t *pcb;
    uint64_t *base;
    char** argv;
} Process;

extern uint64_t* createPCB(uint64_t* wrapper, uint64_t* pcbAddr, uint64_t* entryPoint, int argc, char* argv[]);
extern void scheduleNext();

static Process processes[MAX_PROCESSES] = {0};
static int currentProcess = -1;
static int isSchedulerEnabled = 0;
static int activeProcesses = 0;
static size_t currentProcessQuantums = 0;

//-------------------------------------------------------------------------------------------------------
//Testing process
static size_t var1 = 0;
static size_t var2 = 0;
static size_t var3 = 0;

/*
void testProcess(){
    while(1) {
        if(var1 % 1000000 == 0) {
            print("Process [1] is running\n");
            changeConsoleSide(1);
            print("\n");
            changeConsoleSide(0);
        }
        var1++;
    }
}
*/

void testProcess2(){
    while(1) {
        if(var2 % 1000000 == 0) {
            print("Process [2] is running\n");
            changeConsoleSide(1);
            print("\n");
            changeConsoleSide(0);
        }
        var2++;
    }
}

void testProcess3(){
    while(1) {
        if(var3 % 1000000 == 0) {
            print("Process [3] is running\n");
            changeConsoleSide(1);
            print("\n");
            changeConsoleSide(0);
        }
        var3++;
    }
}
//-------------------------------------------------------------------------------------------------------

void enableScheduler(){
    isSchedulerEnabled = 1;
}

static int getFirstFree(){
    for(int i=0 ; i < MAX_PROCESSES ; i++){
        if(processes[i].state == TERMINATED)
            return i;
    }
    return -1;
}

static void* loadArgv(int argc, char* argv[]){
    size_t argvLen = totalStrlen(argc, argv);
    void* argPtr = memAlloc(argvLen, NO_ACTION);
    memcpy(argPtr, argv, argvLen);
    return argPtr;
}

static void processWrapper(void* processEntry, int argc, char* argv[]){
    int status = ((int(*)(int, char*[]))processEntry)(argc, argv);
    exit(status);
}

int createProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name){
    /*
    print("RECIBIDA (K): ");
    printInt(argv[0], 16);
    print("\n");
    if(priority != SYSTEM){
        print("VALOR (K): ");
        print(argv[0]);
    }
    */
    int processIdx = getFirstFree();
    if(processIdx == -1)
        return processIdx;
    uint64_t* baseAddr = memAlloc(sizeof(uint64_t) * (PCB_REGISTERS + PROCESS_STACK), SET_ZERO);
    processes[processIdx].argv = loadArgv(argc, argv);
    processes[processIdx].pcb = createPCB(processWrapper, baseAddr, entryPoint, argc, processes[processIdx].argv);
    processes[processIdx].base = baseAddr;
    processes[processIdx].state = READY;
    processes[processIdx].priority = priority;
    processes[processIdx].name = name;
    activeProcesses++;
    return processIdx;
}

void exit(int status){
    processes[currentProcess].state = TERMINATED;
    memFree(processes[currentProcess].base);
    memFree(processes[currentProcess].argv);
    activeProcesses--;
    scheduleNext();
}

// TODO
void killRandomProcess(){

}

void printProcess(uint64_t* currentProcPCB) {
    changeConsoleSide(1);
    print("PCB ACTUAL:\n");
    for(int i=0 ; i < 21 ; i++){
        printInt(currentProcPCB+i, 16);
        print("\n");
    }
    print("\n");
    print("\n");
    print("\n");
    changeConsoleSide(0);
}

static int getNextReady(int current){
    while(processes[current+1].state != READY){
        current++;
        if(current+1 == MAX_PROCESSES)
            current = -1;
    }
    return current+1;
}

uint64_t* schedule(uint64_t* currentProcPCB){
    if(isSchedulerEnabled){
        if(currentProcess == -1){       // Only when no processes are running yet (only kernel)
            currentProcessQuantums++;
            return processes[++currentProcess].pcb;
        }
        if(processes[currentProcess].state != TERMINATED){
            processes[currentProcess].pcb = currentProcPCB;
        }
        if(processes[currentProcess].state == TERMINATED || processes[currentProcess].priority < currentProcessQuantums){
            currentProcessQuantums = 0;
            currentProcess = getNextReady(currentProcess);
        }
        currentProcPCB = processes[currentProcess].pcb;
        currentProcessQuantums++;
        // printProcess(processes[currentProcess].pcb);
    }
    return currentProcPCB;
}

