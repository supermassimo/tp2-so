#include <scheduler.h>

#define MAX_PROCESSES  10
#define PCB_REGISTERS  21
#define PROCESS_STACK  400

typedef enum State {READY, BLOCKED, SUSPENDED, TERMINATED} State;

typedef struct {
    State state;
    int priority;
    uint64_t* pcb;
} Process;

extern uint64_t* createPCB(uint64_t* entryPoint, uint64_t* pcbAddr);

static Process processes[MAX_PROCESSES];
static int activeProcesses = 0;
static int currentProcess = -1;
static int isSchedulerEnabled = 0;

//-------------------------------------------------------------------------------------------------------
//Testing process
static size_t var1 = 0;
static size_t var2 = 0;
static size_t var3 = 0;


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

int createProcess(void* entryPoint, int argc, char** argv){
    uint64_t* pcbAddr = memAlloc(sizeof(uint64_t) * (PCB_REGISTERS + PROCESS_STACK), SET_ZERO);
    processes[0].pcb = createPCB(entryPoint, pcbAddr);
    activeProcesses++;
    return 0;
}

void killProcess(){
    
}

void printProcess(uint64_t* currentProcPCB) {
    changeConsoleSide(1);
    print("PCB ACTUAL:\n");
    for(int i=0 ; i < 21 ; i++){
        printInt(currentProcPCB[i], 16);
        print("\n");
    }
    print("\n");
    print("\n");
    changeConsoleSide(0);
}

uint64_t* schedule(uint64_t* currentProcPCB){
    if(isSchedulerEnabled){
        if(currentProcess != -1){
            processes[currentProcess].pcb = currentProcPCB;
        }
        if(currentProcess+1 == activeProcesses){
            currentProcess = -1;
        }
        currentProcPCB = processes[++currentProcess].pcb;
        // printProcess(currentProcPCB);
    }
    return currentProcPCB;
}

