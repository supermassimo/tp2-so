#include <scheduler.h>
#include <kernel.h>

#define MAX_PROCESSES 10

typedef enum State {READY, BLOCKED, SUSPENDED, TERMINATED} State;

typedef struct {
    uint64_t* pcb;
} Process;

extern uint64_t* createPCB(uint64_t* entryPoint, uint64_t* pcbAddr);

static Process processes[MAX_PROCESSES];
static int activeProcessIdx = 0;

//-------------------------------------------------------------------------------------------------------
//Testing process
static size_t var1 = 0;
static size_t var2 = 0;
static size_t var3 = 0;


void testProcess(){
    while(1) {
        if(var1 % 1000000 == 0) {
            print("Process [1] is running\n");
        }
        var1++;
    }
}

void testProcess2(){
    while(1) {
        if(var2 % 1000000 == 0) {
            print("Process [2] is running\n");
        }
        var2++;
    }
}

void testProcess3(){
    while(1) {
        if(var3 % 1000000 == 0) {
            print("Process [3] is running\n");
        }
        var3++;
    }
}
//-------------------------------------------------------------------------------------------------------

void createProcess(){
    uint64_t* pcbAddr = memAlloc(sizeof(uint64_t) * 21, NO_ACTION);
    processes[1].pcb = createPCB(testProcess, pcbAddr);

    uint64_t* pcbAddr2 = memAlloc(sizeof(uint64_t) * 21, NO_ACTION);
    processes[2].pcb = createPCB(testProcess2, pcbAddr2);

    uint64_t* pcbAddr3 = memAlloc(sizeof(uint64_t) * 21, NO_ACTION);
    processes[3].pcb = createPCB(testProcess3, pcbAddr3);
}

void printProcess(uint64_t* currentProcPCB) {
    changeConsoleSide(1);
    print("PCB ACTUAL: ");
    printInt((long int)currentProcPCB, 16);
    print("\n");    
    print("Kernel: ");
    printInt((long int)processes[0].pcb, 16);
    print("\n");
    print("PCB[1]: ");
    printInt((long int)processes[1].pcb, 16);
    print("\n");
    print("PCB[2]: ");
    printInt((long int)processes[2].pcb, 16);
    print("\n");
    print("PCB[3]: ");
    printInt((long int)processes[3].pcb, 16);
    print("\n");
    print("\n");    
    print("\n"); 
    changeConsoleSide(0);
}

uint64_t* schedule(uint64_t* currentProcPCB){
    if(isSchedulerEnabled()){
        if(activeProcessIdx == 0) {
            processes[0].pcb = currentProcPCB;
            printProcess(currentProcPCB);
            activeProcessIdx++;
            return processes[1].pcb;   
        }
        if(activeProcessIdx == 1) {
            printProcess(currentProcPCB);
            processes[1].pcb = currentProcPCB;
            activeProcessIdx++;
            return processes[0].pcb;   
        }
        if(activeProcessIdx == 2) {
            printProcess(currentProcPCB);
            processes[0].pcb = currentProcPCB;
            activeProcessIdx--;
            return processes[1].pcb;   
        }
    }
    return currentProcPCB;
}

        
        
    