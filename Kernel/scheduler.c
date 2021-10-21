#include <scheduler.h>

#define MAX_PROCESSES  10
#define PCB_REGISTERS  21
#define PROCESS_STACK  400

typedef enum State {TERMINATED, READY, BLOCKED, SUSPENDED} State;

typedef struct {
    State state;
    Priority priority;
    uint64_t *pcb;
    uint64_t *stack;
} Process;

extern uint64_t* createPCB(uint64_t* entryPoint, uint64_t* pcbAddr, uint64_t* stackAddr, int argc, char** argv);
extern void scheduleNext();

static Process processes[MAX_PROCESSES] = {0};
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

static int getFirstFree(){
    for(int i=0 ; i < MAX_PROCESSES ; i++){
        if(processes[i].state == TERMINATED)
            return i;
    }
    return -1;
}

int createProcess(void* entryPoint, Priority priority, int argc, char** argv){
    int processIdx = getFirstFree();
    if(processIdx == -1)
        return processIdx;
    uint64_t* pcbAddr = memAlloc(sizeof(uint64_t) * PROCESS_STACK, SET_ZERO);
    uint64_t* stackAddr = memAlloc(sizeof(uint64_t) * PROCESS_STACK, SET_ZERO);
    processes[processIdx].pcb = createPCB(entryPoint, pcbAddr, stackAddr, argc, argv);
    processes[processIdx].stack = stackAddr;
    processes[processIdx].state = READY;
    processes[processIdx].priority = priority;
    return processIdx;
}

void killCurrentProcess(){
    processes[currentProcess].state = TERMINATED;
    memFree(processes[currentProcess].stack);
    memFree(processes[currentProcess].pcb);
    scheduleNext();
}

// TODO
void killRandomProcess(){

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
    print("\n");
    changeConsoleSide(0);
}

uint64_t* schedule(uint64_t* currentProcPCB){
    if(isSchedulerEnabled){
        if(currentProcess != -1 && processes[currentProcess].state != TERMINATED){
            processes[currentProcess].pcb = currentProcPCB;
        }
        while(processes[currentProcess+1].state != READY){
            currentProcess++;
            if(currentProcess+1 == MAX_PROCESSES)
                currentProcess = -1;
        }
        currentProcPCB = processes[++currentProcess].pcb;
        // printProcess(processes[0].pcb);
    }
    return currentProcPCB;
}

