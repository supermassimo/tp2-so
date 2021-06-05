#include "./include/commands.h"
#include "./include/mystdio.h"
#include "./include/mystdlib.h"

extern void writeRegistries();
extern void writeMemContent(char* startPos, size_t amount);
extern void writeDateTime(int utc);
extern void invalidOpcodeThrower();
extern void setIdle(int idle);
extern void clear();

typedef struct commandStruct{
    char* name;
    void* handler;
} commandStruct;

typedef struct helpStruct{
    char* name;
    char* help_message;
} helpStruct;

typedef struct exceptionTestStruct{
    char* exception;
    void* thrower;
} exceptionTestStruct;

static const size_t commandAmount = 8;
static const size_t exceptionAmount = 2;

static void echoHandler(char params[][MAX_PARAMETER_LENGTH], int paramAmount){
    if(paramAmount < 1){
        printErr("Missing parameter for command 'echo'\n");
        return;
    }
    size_t finalLength = 0, actualLength;
    for(int i=0 ; i < paramAmount ; i++){
        finalLength += strlen(params[i]) + 1;
    }
    char output[finalLength];
    actualLength = concatStrings(params, paramAmount, output);
    printf(output);
    printf("\n");
    if(actualLength != finalLength - 1){            // finalLength computes the '\0' while actualLength doesn't
        printErr("Error concating the strings\n");
        return;
    }
}

static void inforegHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many arguments for command 'inforeg'\n");
        return;
    }
    writeRegistries();
}

static void printmemHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 1){
        printErr("Missing parameter for command 'printmem'\n");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many arguments for command 'printmem'\n");
        return;
    }
    char *memPos = strToNumPos(params[0]);
    writeMemContent(memPos, 32);
}

static void datetimeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 1){
        printErr("Missing parameter for command 'daytime'\n");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many arguments for command 'datetime'\n");
        return;
    }
    long utc = strToNum(params[0]);
    // TODO: Fix code below (takes utc as unsigned number)
    // printInt(utc, 10000, 10);
    // if(utc < 12 || utc > 12){
    //     printErr("Invalid utc value");
    //     return;
    // }
    writeDateTime(utc);
}

static void clearHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many arguments for command 'clear'\n");
        return;
    }
    clear();
}

//help must be at the top
static helpStruct help_messages[] = {
    {"help", "'help': Get information on how to use commands\nUse: 'help [command]'\n'command': Command to get use information about\n"},
    {"echo", "'echo': Print a message on the console\nUse: 'echo [message]'\n'message': Message to print in console\n"},
    {"inforeg", "'inforeg': Print the states of the registries\nUse: 'inforeg'\n"},
    {"printmem", "'printmem': Print the first 32 bytes following a place in memory\nUse: 'printmem [pointer]'\n'pointer': Memory address of first byte to print\n"},
    {"datetime", "'datetime': Print the time and date for a specific timezone\nUse: 'datetime [timezone]'\n'timezone': Timezone to print the current time of\n"},
    {"localdatetime", "'localdatetime': Print the local time and date\nUse: 'localdatetime'\n"},
    {"test", "'test': Throws the provided exception\nUse: 'test [exception]'\n''exception': Type of exception to be thrown\n"},
    {"clear", "'clear': Clears the current console\nUse: 'clear'\n"}
};

static void helpHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if (paramAmount > 1){
        printErr("Too many arguments for command 'help'\nUse: help [command]\n");
    }
    if (paramAmount == 0){
        printf(help_messages[0].help_message);
    }
    for(int i=0 ; i < commandAmount ; i++){
        if(strcmp(help_messages[i].name, params[0]) == 0){
            printf(help_messages[i].help_message);
        }
    }
}

static void localDateTimeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many arguments for command 'localdatetime'");
        return;
    }
    writeDateTime(LOCAL_UTC);
}

static void divByZeroThrower(){
    int value = 1/0;
    return;
}

static exceptionTestStruct exceptions[] = {
    {"div-by-zero", &divByZeroThrower},
    {"invalid-opcode", &invalidOpcodeThrower}
};

static void testHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 0){
        printErr("Missing argument for command 'test'");
    }
    if(paramAmount > 1){
        printErr("Too many arguments for command 'test'");
    }
    for(int i=0 ; i < exceptionAmount ; i++){
        if(strcmp(params[0], exceptions[i].exception) == 0)
            ((void(*)())exceptions[i].thrower)();
    }
}

static commandStruct commands[] = {
    {"help", &helpHandler},
    {"echo", &echoHandler},
    {"inforeg", &inforegHandler},
    {"printmem", &printmemHandler},
    {"datetime", &datetimeHandler},
    {"localdatetime", &localDateTimeHandler},
    {"test", &testHandler},
    {"clear", &clearHandler}
};

static int getCommandAndParams(char* command, char params[][MAX_PARAMETER_LENGTH], char* input){
    int inputIdx, j=0, paramIdx=0;
    for(inputIdx=0 ; input[inputIdx] != ' ' && input[inputIdx] != 0 ; inputIdx++){
        command[inputIdx] = input[inputIdx];
    }
    command[inputIdx] = 0;
    if(input[inputIdx] != 0){
        inputIdx++;
        while(input[inputIdx] != 0){
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
    char commandName[MAX_COMMAND_LENGTH] = "";
    char params[MAX_COMMAND_AMOUNT][MAX_PARAMETER_LENGTH];
    int paramAmount = getCommandAndParams(commandName, params, string);
    for(int i=0 ; i < commandAmount ; i++){
        if(strcmp(commands[i].name, commandName) == 0){
            setIdle(0);
            ((void(*)(char[][MAX_PARAMETER_LENGTH], size_t))commands[i].handler)(params, paramAmount);
            setIdle(1);
            return;
        }
    }
    printErr("Unknown command. Use 'help' for a list of commands");
}