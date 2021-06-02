#include "./include/mystdio.h"

#define COMMAND_AMOUNT 2

extern void writeRegistries();

typedef struct commandStruct{
    char* name;
    void* handler;
} commandStruct;

static void echoHandler(char* string){
    printf(string, strlen(string));
}

static void inforegHandler(){
    writeRegistries();
}

static commandStruct commands[] = {
    {"echo", &echoHandler},
    {"inforeg", &inforegHandler}
};

static void getCommandAndParams(char* command, char** params, char* nameWithParams){
    int i=0;
    for(i=0 ; nameWithParams[i] != ' ' && nameWithParams[i] != 0 ; i++){
        command[i] = nameWithParams[i];
    }
    command[i] = 0;
    if(nameWithParams[i] != 0){
        int paramIdx = 0;
        for(int j=i ; nameWithParams[j] != 0 ; j++){
            if(nameWithParams[j] == ' '){
                params[paramIdx++] = 0;
                continue;
            }
            params[paramIdx][j] = nameWithParams[j];
        }
    }
}

void commandHandler(char* string){
    char commandName[20];
    char params[10][100];
    getCommandAndParams(commandName, params, string);
    for(int i=0 ; i < COMMAND_AMOUNT ; i++){
        if(strcmp(commands[i].name, commandName) == 0){
            (void*)commands[i].handler;
        }
    }
}