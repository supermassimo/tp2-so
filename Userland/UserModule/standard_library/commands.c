// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/commands.h"
#include "./include/mystdio.h"
#include "./include/mystdlib.h"

/*              TEST MM HEADERS             */
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "./include/test_util.h"
/*******************************************/

extern void getRegistries(Registries* regs);
extern void getMemContent(uint64_t startPos, uint8_t* target, size_t amount);
extern void invalidOpcodeThrower();
extern void setIdle(int idle);
extern void clear();
extern bool getCpuFeatures(CommonFeatures* commonFeatures, ExtendedFeatures* extendedFeatures);
extern int sleep(int pid, size_t seconds);
extern int getQuadratic(float a, float b, float c, float*, float*);
extern void getDateTime(Date* date, Time* time, int utc);
extern void getMemInfo(MemoryInfo* meminfo);
extern int createProcess(void* entryPoint, UserPriority priority, int argc, char* argv[], char* name);
extern void exit(int status);
extern int kill(int pid, ProcessSignal sig);
extern int nice(int pid, UserPriority priority);
extern int block(int pid);
extern void printAllProcesses();
extern int getpid();
extern void skip();

#define PRINTMEM_BYTES 32

typedef enum CommandType {BUILT_IN, PROCESS} CommandType;

typedef struct commandStruct{
    char* name;
    void* handler;
    char* help_message;
    CommandType type;
} commandStruct;

typedef struct exceptionTestStruct{
    char* exception;
    void* thrower;
} exceptionTestStruct;

static const size_t commandAmount = 25;
static const size_t exceptionAmount = 2;

#define QUADRATIC_PRECISION 2
#define FLOAT_STRING_SIZE 100

static void echoHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 1){
        printErr("Missing parameter for command 'echo'");
        return;
    }
    size_t finalLength = 0, actualLength;
    for(int i=0 ; i < paramAmount ; i++){
        finalLength += strlen(params[i]) + 1;
    }
    char output[finalLength + 1];
    actualLength = concatStrings(params, paramAmount, output);
    printf(output);
    printf("\n");
    if(actualLength != finalLength - 1){            // finalLength computes the '\0' while actualLength doesn't
        printErr("Error concating the strings");
        return;
    }
}

static void cpufeaturesHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'cpufeatures'");
        return;
    }
    CommonFeatures commonFeatures;
    ExtendedFeatures extendedFeatures;
    if(!getCpuFeatures(&commonFeatures, &extendedFeatures)){
        printErr("cpuid instruction not supported");
        return;
    }
    printFeatures(commonFeatures, extendedFeatures);
    return;
}

static void inforegHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'inforeg'");
        return;
    }
    Registries regs;
    getRegistries(&regs);
    if (regs.RIP == 0){
        printErr("The registries have not been captured yet. press the [~] key to capture before executing 'inforeg'.");
    } else {
        printRegistries(regs);
    }
    return;
}

static void printmemHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 1){
        printErr("Missing parameter for command 'printmem'");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many parameters for command 'printmem'");
        return;
    }
    uint64_t memPos = strToNumPos(params[0]);
    if((int64_t)memPos == -1)
        return;
    char memContent[PRINTMEM_BYTES];
    getMemContent(memPos, (uint8_t*)memContent, PRINTMEM_BYTES);
    printMemContent(memPos, (uint8_t*)memContent, PRINTMEM_BYTES);
}

static void memHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 1){
        printErr("Too many parameters for command 'mem'");
        return;
    }
    MemoryInfo meminfo;
    getMemInfo(&meminfo);
    printMemInfo(meminfo, params[0]);
}

// Previously used to test div-by-zero exception
static void divByZeroThrower(){
    return 1/0;
}

static exceptionTestStruct exceptions[] = {
    {"div-by-zero", &divByZeroThrower},
    {"invalid-opcode", &invalidOpcodeThrower}
};

// for testing purposes
static void testallocHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 1){
        printErr("Missing parameter for command 'testalloc'");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many parameters for command 'testalloc'");
        return;
    }
    int num = strToNum(params[0]);
    void* mem = malloc(sizeof(int));
    if(mem == NULL)
        return;
    *((int*)mem) = num;

    int num2 = *((int*)mem);
    printInt(num2, 10, 10);
    printf("\nMemory allocated at: ");
    printInt((long)mem, 16, 16);
    printf("\n");
    /* calloc test */
    /*
        uint8_t* mem = (uint8_t*)calloc(10, 10);
        for(int i=0 ;  ; i++){
            if(i == 100){
                printf("All of memory was initialized to zero\n");
                break;
            }
            if(*mem != 0){
                printf("Memory was not fully initialized to zero\n");
                break;
            }
        }
    */
    free(mem);
    return;
}

static void testHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 0){
        printErr("Missing parameter for command 'test'");
    }
    if(paramAmount > 1){
        printErr("Too many parameters for command 'test'");
    }
    for(int i=0 ; i < exceptionAmount ; i++){
        if(strcmp(params[0], exceptions[i].exception) == 0)
            ((void(*)())exceptions[i].thrower)();
    }
}

void psHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'ps'");
        return;
    }
    printAllProcesses();
    return;
}

int loop(int argc, char* argv[]) {
    int pid = getpid();
    printf("Recibo ");
    printf(argv[0]);
    printf("segundos\n");
    size_t seconds = strToNum(argv[0]);
    printf("Mando ");
    printInt(seconds, 10, 10);
    printf("segunds\n");
    // size_t seconds = 5;
    while(1){
        sleep(pid, seconds);
        printf("Hi, process ");
        printInt(pid,2,10);
        printf(" here\n");
    }
    return 0;
    /*
    for(size_t i=0 ; i < 10000000000 ; i++){
        if(i % 100000000 == 0) {
            printf("Hi, process ");
            printInt(pid,2,10);
            printf(" here\n");
        }
    }
    */
}

void loopHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 0){
        printErr("Missing parameter for command 'loop'");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many parameters for command 'loop'");
        return;
    }
    char* time[paramAmount];
    time[0] = params[0];
    if(createProcess(loop, LOW, 1, time, "loop") == -1){
        printErr("Cannot create a new process; process limit reached");
        return;
    }
    return;
}

void killHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 0){
        printErr("Missing parameter for command 'kill'");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many parameters for command 'kill'");
        return;
    }
    int pid = strToNum(params[0]);
    if(kill(pid, SIG_KILL) == -1){
        printErr("No process with pid sent");
        return;
    }
}

void niceHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 0){
        printErr("Missing parameters for command 'nice'");
        return;
    }
    if(paramAmount == 1){
        printErr("Missing process new priority");
        return;
    }
    if(paramAmount > 2){
        printErr("Too many parameters for command 'nice'");
        return;
    }
    int pid = strToNum(params[0]);
    int priority = strToNum(params[1]);
    if(nice(pid, priority) == -1){
        printErr("Cannot assign new priority to process with pid sent");
        return;
    }
}

void blockHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 0){
        printErr("Missing parameters for command 'block'");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many parameters for command 'block'");
        return;
    }
    int pid = strToNum(params[0]);
    if(block(pid) == -1){
        printErr("Cannot change sent process state");
        return;
    }
}

void skipHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount != 0){
        printErr("Too many parameters for command 'skip'");
        return;
    }
    skip();
}

void semHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'sem'");
        return;
    }
    printf("Here comes sem\n");
    return;
}

void catHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'cat'");
        return;
    }
    printf("Here comes cat\n");
    return;
}

void wcHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'wc'");
        return;
    }
    printf("Here comes wc\n");
    return;
}

void filterHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'filter'");
        return;
    }
    printf("Here comes filter\n");
    return;
}

void pipeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'pipe'");
        return;
    }
    printf("Here comes pipes\n");
    return;
}

void phyloHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'phylo'");
        return;
    }
    printf("Here comes phylo\n");
    return;
}

static void sleepHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 2){
        printErr("Missing parameters for command 'sleep'");
        return;
    }
    if(paramAmount > 2){
        printErr("Too many parameters for command 'sleep'");
        return;
    }
    int pid = strToNum(params[0]);
    size_t seconds = strToNum(params[1]);
    if(sleep(pid, seconds) == -1) {
        printErr("Cannot sleep given process pid");
        return;
    }
}

static void clearHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'clear'");
        return;
    }
    clear();
}

int testProcess(int argc, char* argv[]){
    printf("PID: ");
    printInt(getpid(), 10, 10);
    printf("\n");
    printf("(T) RECIBO:\n");
    printf("- argv[0]: ");
    printf(argv[0]);
    printf("\n");
    printf("- argv[1]: ");
    printf(argv[1]);
    printf("\n");
    return 0;
}

int testProcessA(int argc, char* argv[]){
    for(size_t i=0 ; i < 10000000000 ; i++){
        if(i % 100000000 == 0)
            printf("A");
    }
    return 0;
}

int testProcessB(int argc, char* argv[]){
    for(size_t i=0 ; i < 10000000000 ; i++){
        if(i % 100000000 == 0)
            printf("B");
    }
    return 0;
}

void testProcessHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 1){
        printErr("Too many parameters for command 'test'");
        return;
    }
    if(paramAmount == 1 && strcmp("b", params[0]) != 0){
        printErr("Wrong parameter sent. Check 'help testprocess' for more info");
        return;
    }
    char* msg[] = {"Estoy vivo\n", "Me muero\n"};
    printf("MANDADA: ");
    printInt(msg[0], 100, 16);
    printf("\n");
    if(paramAmount == 0)
        testProcess(2, msg);
    else {
        /*
        if(createProcess(testProcess, HIGH, 2, msg, "testProcess") == -1){
            printErr("Cannot create a new process; process limit reached");
            return;
        }
        */
        if(createProcess(testProcessB, HIGH, 2, msg, "testPB") == -1){
            printErr("Cannot create a new process; process limit reached");
            return;
        }
        if(createProcess(testProcessA, HIGH, 2, msg, "testPA") == -1){
            printErr("Cannot create a new process; process limit reached");
            return;
        }
    }
}

/****               TEST_MM             ****/
#define MAX_BLOCKS 10
#define MAX_MEMORY 0x19266666 //80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

int test_mm(int argc, char* argv[]){
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint64_t total;

    // while (1){
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
        mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
        printf("Request block #");
        printInt(rq, 10, 10);
        printf(" of 0x");
        printInt(mm_rqs[rq].size, 10, 16);
        printf("b\n");
        mm_rqs[rq].address = malloc(mm_rqs[rq].size); // TODO: Port this call as required
        if(mm_rqs[rq].address == NULL){
            printErr("No more memory left\n");
            return -1;
        }
        printf("on addr: ");
        printInt(mm_rqs[rq].address, 10, 16);
        printf("\n");
        //TODO: check if NULL
        total += mm_rqs[rq].size;
        rq++;
    }

    // Set
    for (uint32_t i = 0; i < rq; i++){
        printf("Setting block #");
        printInt(i, 10, 10);
        printf("\n");
        if (mm_rqs[i].address != NULL){
            for(int j=0 ; j < mm_rqs[i].size ; j++)
                *(char*)(mm_rqs[i].address+j) = i;
        }
        printf("Block #");
        printInt(i, 11, 10);
        printf(" was successfully set\n");
    }

    // Check
    for (uint32_t i = 0; i < rq; i++){
        if (mm_rqs[i].address != NULL){
            if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
                printf("ERROR checking block #");
                printInt(i, 10, 10);
                printf("\n");
            }
            else{
                printf("Block #");
                printInt(i, 10, 10);
                printf(" successfully checked\n");
            }
        }
    }

    // Free
    for (uint32_t i = 0; i < rq; i++){
        if (mm_rqs[i].address != NULL){
            free(mm_rqs[i].address);
            printf("Freeing block #");
            printInt(i, 10, 10);
            printf("\n");
        }
    }
  // }
    printf("FINISH testMM\n");
    return 0; 
}

static void testMMHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'testmm'");
        return;
    }
    char *args[1] = {"Hola\n"};
    createProcess(test_mm, HIGH, 1, args, "testMM");
}

static void printCommandTypeMessage(commandStruct command){
    printf(command.name);
    printf(" is a ");
    switch(command.type){
        case BUILT_IN:
            printf("shell built-in");
            break;
        case PROCESS:
            printf("process");
            break;
        default:
            break;
    }
    printf("\n");
}

static void typeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount);
static void helpHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount);

static commandStruct commands[] = {
    {"help", &helpHandler, "'help': Get information on how to use commands\nUse: 'help [command]'\n'command': Command to get use information about\n", BUILT_IN},
    {"echo", &echoHandler, "'echo': Print a message on the console\nUse: 'echo [message]'\n'message': Message to print in console\n", BUILT_IN},
    {"cpufeatures", &cpufeaturesHandler, "'cpufeatures': Print cpu support for key features like mmx, sse, avx, etc\nUse: 'cpufeatures'\n", BUILT_IN},
    {"exception", &testHandler, "'exception': Throws the provided exception\nUse: 'test [exception]'\n'exception': Type of exception to be thrown\nAvailable exceptions:\ndiv-by-zero\ninvalid-opcode\n", BUILT_IN},
    {"inforeg", &inforegHandler, "'inforeg': Print the states of the registries\nUse: 'inforeg'\n", BUILT_IN},
    {"printmem", &printmemHandler, "'printmem': Print the first 32 bytes following a place in memory\nUse: 'printmem [pointer]'\n'pointer': Memory address of first byte to print\n", BUILT_IN},
    {"mem", &memHandler, "'mem': Displays memory info\nUse: 'mem' [units]\n'units': Determines which unit the info will be displayed on\nOptions:\n-b: Bytes (Default)\n-k: Kilobytes\n", BUILT_IN},
    {"testalloc", &testallocHandler, "'testalloc': Tests the functionality of memory allocation\nUse: 'testalloc' [test num]\n'test num': A test integer number that will be saved in memory and then read\n", BUILT_IN},
    {"ps", &psHandler, "'ps': Displays running process information\n", BUILT_IN},
    {"loop", &loopHandler, "'loop': Displays process Id and a greeting every few seconds", PROCESS},
    {"kill", &killHandler, "'kill': Kills process with pid sent\nUse: 'kill' [pid]\n'pid': Id of process\n", BUILT_IN},
    {"nice", &niceHandler, "'nice': Change priority of given process\nUse: 'nice' [pid] [priority]\n'pid': Id of process\n'priority': New priority to assign to process\n", BUILT_IN},
    {"block", &blockHandler, "'block': Toggles a process state between ready and blocked\nUse: 'block [pid]'\n'pid': Id of process\n", BUILT_IN},
    {"skip", &skipHandler, "'skip': Skips execution of current process. Do not confound with 'kill'\nUse: 'skip'\n", BUILT_IN},
    {"sem", &semHandler, "'sem': Displays current semaphores information\nUse: sem [?] ...", BUILT_IN},
    {"cat", &catHandler, "'cat': ", PROCESS},
    {"wc", &wcHandler, "'wc': ", PROCESS},
    {"filter", &filterHandler, "'filter': ", PROCESS},
    {"pipe", &pipeHandler, "'pipe': ", BUILT_IN},
    {"phylo", &phyloHandler, "'phylo': ", PROCESS},
    {"clear", &clearHandler, "'clear': Clears the current console\nUse: 'clear'\n", BUILT_IN},
    {"sleep", &sleepHandler, "'sleep': Causes the given process to sleep for the seconds specified\nUse: 'sleep [pid] [seconds]'\n'pid': Process id\n'seconds': Number of seconds for the system to sleep\n", BUILT_IN},    
    {"testprocess", &testProcessHandler, "'testprocess': Creates new process\nUse: 'testprocess'\n", PROCESS},
    {"type", &typeHandler, "'type': Prints a command type\nUse: 'type [command]'\n'command': Command name\n", BUILT_IN},
    {"testmm", &testMMHandler, "", PROCESS}
};

static void typeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 0){
        printErr("Missing parameter for command 'type'");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many parameters for command 'type'");
        return;
    }
    for(int i=0 ; i < commandAmount ; i++){
        if(strcmp(commands[i].name, params[0]) == 0){
            printCommandTypeMessage(commands[i]);
            return;
        }
    }
    printErr("No command with sent name was found");
    return;
}

static void helpHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if (paramAmount > 1){
        printErr("Too many parameters for command 'help'\nUse: help [command]");
    }
    if (paramAmount == 0){
        printf("Available Commands:\n");
        printf("help [command]\n");
        for(int i=1; i<commandAmount; i++) {
            printf(commands[i].name);
            printf("\n");
        }
    }
    for(int i=0 ; i < commandAmount ; i++){
        if(strcmp(commands[i].name, params[0]) == 0){
            printf(commands[i].help_message);
        }
    }
}

static int isEnd(int c){
    if (c == '\n' || c == 0) return 1;
    return 0;
}

static int getCommandAndParams(char* command, char params[][MAX_PARAMETER_LENGTH], char* input){
    int inputIdx, paramIdx=0;
    for(inputIdx=0 ; input[inputIdx] != ' ' && !isEnd(input[inputIdx]); inputIdx++){
        if (inputIdx < MAX_COMMAND_LENGTH)
            command[inputIdx] = input[inputIdx];
    }
    command[inputIdx] = 0;
    if(!isEnd(input[inputIdx])){
        int j=0;
        inputIdx++;
        while(!isEnd(input[inputIdx])){
            if(input[inputIdx] == ' '){
                params[paramIdx++][j] = 0;
                j = 0;
                inputIdx++;
                continue;
            }
            params[paramIdx][j++] = input[inputIdx++];
        }
        params[paramIdx++][j] = 0;
    }
    return paramIdx;
}

void commandHandler(char* string){
    char commandName[MAX_COMMAND_LENGTH+1] = "";
    char params[MAX_PARAMETER_AMOUNT][MAX_PARAMETER_LENGTH];
    int paramAmount = getCommandAndParams(commandName, params, string);
    for(int i=0 ; i < commandAmount ; i++){
        if(strcmp(commands[i].name, commandName) == 0){
            setIdle(0);
            ((void(*)(char[][MAX_PARAMETER_LENGTH], size_t))commands[i].handler)(params, paramAmount);
            printf("> ");
            setIdle(1);
            return;
        }
    }
    printErr("Unknown command. Use 'help' for a list of commands");
    printf("> ");
}