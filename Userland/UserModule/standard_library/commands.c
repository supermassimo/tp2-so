#include "./include/mystdio.h"
#include "./include/mystdlib.h"

#define MAX_COMMAND_AMOUNT 10
#define MAX_COMMAND_LENGTH 20
#define MAX_PARAMETER_LENGTH 200

extern void writeRegistries();

typedef struct commandStruct{
    char* name;
    void* handler;
} commandStruct;

const size_t commandAmount = 2;

static void echoHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmoumt){
    if(paramAmoumt < 1){
        printErr("Missing parameter for command echo");
        return;
    }
    // size_t finalLength = 0;
    // for(int i=0 ; i < paramAmoumt ; i++){
    //    finalLength += strlen(params[i]) + 1;
    // }
    // printInt(finalLength, 10, 10);
    // char output[finalLength];
    // concatStrings(params, paramAmoumt, output);
    printf(params[0]);
}

static void inforegHandler(char params[][MAX_PARAMETER_LENGTH] , size_t paramAmoumt){
    writeRegistries();
}

static commandStruct commands[] = {
    {"echo", &echoHandler},
    {"inforeg", &inforegHandler}
};

static int getCommandAndParams(char* command, char params[][MAX_PARAMETER_LENGTH], char* input){
    int inputIdx, j=0, paramIdx = 0;
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
        // for(j=0 ; nameWithParams[i] != 0 ; j++){
        //     if(nameWithParams[i] == ' '){
        //         params[paramIdx++][j] = 0;
        //         j = -1;
        //         i++;
        //         continue;
        //     }
        //     params[paramIdx][j] = nameWithParams[i++];
        // }
    }
    return paramIdx;
}

void commandHandler(char* string){
    char commandName[MAX_COMMAND_LENGTH];
    char params[MAX_COMMAND_AMOUNT][MAX_PARAMETER_LENGTH];
    int paramAmount = getCommandAndParams(commandName, params, string);
    for(int i=0 ; i < commandAmount ; i++){
        if(strcmp(commands[i].name, commandName) == 0){
            ((void(*)(char[][MAX_PARAMETER_LENGTH], size_t))commands[i].handler)(params, paramAmount);
        }
    }
}