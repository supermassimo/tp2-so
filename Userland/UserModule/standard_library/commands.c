#include "./include/commands.h"
#include "./include/mystdio.h"
#include "./include/mystdlib.h"

extern void writeRegistries();
extern void writeMemContent(char* startPos, size_t amount);
extern void writeDateTime(int utc);
extern void invalidOpcodeThrower();
extern void setIdle(int idle);
extern void clear();
extern void writeCpuFeatures();
extern void sleep(long seconds);
extern int getQuadratic(float a, float b, float c, float* out);


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

static const size_t commandAmount = 11;
static const size_t exceptionAmount = 2;

#define QUADRATIC_PRECISION 2
#define FLOAT_STRING_SIZE 100

static void printQuadratic(float a, float b, float c){
    printFloat(a, FLOAT_STRING_SIZE, QUADRATIC_PRECISION, 10);
    printf("x^2 + ");
    printFloat(b, FLOAT_STRING_SIZE, QUADRATIC_PRECISION, 10);
    printf("x + ");
    printFloat(c, FLOAT_STRING_SIZE, QUADRATIC_PRECISION, 10);
}

static void quadraticHandler(char params[][MAX_PARAMETER_LENGTH], int paramAmount){
    if(paramAmount > 3){
        printErr("Too many parameters for command 'quadratic'\n");
        return;
    }
    if(paramAmount < 3){
        printErr("Missing parameters for command 'quadratic'\n");
        return;
    }

    float a,b,c;
    float roots[2];

    strToFloat(params[0], &a);
    strToFloat(params[1], &b);
    strToFloat(params[2], &c);

    int found = getQuadratic(a, b, c, roots);

    if (found == 0){
        printf("No roots found for function ");
        printQuadratic(a, b, c);
    } else if (found == 1){
        printf("Function ");
        printQuadratic(a, b, c);
        printf(" has one real root:\nx = ");
        printFloat(roots[0], FLOAT_STRING_SIZE, QUADRATIC_PRECISION, 10);
    } else if (found == 2){
        printf("Function ");
        printQuadratic(a, b, c);
        printf(" has two real roots:\nx = ");
        printFloat(roots[0], FLOAT_STRING_SIZE, QUADRATIC_PRECISION, 10);
        printf("\nx = ");
        printFloat(roots[1], FLOAT_STRING_SIZE, QUADRATIC_PRECISION, 10);
    }
    printf("\n");

}

// for testing purposes
static void echofloatHandler(char params[][MAX_PARAMETER_LENGTH], int paramAmount){
    if(paramAmount < 2){
        printErr("Missing parameters for command 'echofloat'");
        return;
    }
    if(paramAmount > 2){
        printErr("Too many parameters for command 'echofloat'");
        return;
    }

    int precision = strToNumPos(params[0]);
    float value;
    if (strToFloat(params[1], &value) != -1){
        printFloat(value, FLOAT_STRING_SIZE, precision, 10);
        printf("\n");
    }
}

static void echoHandler(char params[][MAX_PARAMETER_LENGTH], int paramAmount){
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

static void inforegHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'inforeg'");
        return;
    }
    writeRegistries();
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
    char *memPos = strToNumPos(params[0]);
    if((int64_t)memPos == -1)
        return;
    writeMemContent(memPos, 32);
}

static void datetimeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 1){
        printErr("Missing parameter for command 'daytime'");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many parameters for command 'datetime'");
        return;
    }
    long utc = strToNum(params[0]);
    if(utc < -12 || utc > 12){
        printErr("Invalid utc value");
        return;
    }
    writeDateTime(utc);
}

static void clearHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'clear'");
        return;
    }
    clear();
}

static helpStruct help_messages[] = {
    {"help", "'help': Get information on how to use commands\nUse: 'help [command]'\n'command': Command to get use information about\n"},
    {"echo", "'echo': Print a message on the console\nUse: 'echo [message]'\n'message': Message to print in console\n"},
    {"echofloat", "'echofloat': Prints a floating point number on the console\nUse: 'echo [precision] [number]'\n'precision': Ammount of numbers after the point\n'message': Number to print\n"},
    {"inforeg", "'inforeg': Print the states of the registries\nUse: 'inforeg'\n"},
    {"printmem", "'printmem': Print the first 32 bytes following a place in memory\nUse: 'printmem [pointer]'\n'pointer': Memory address of first byte to print\n"},
    {"datetime", "'datetime': Print the time and date for a specific timezone\nUse: 'datetime [timezone]'\n'timezone': Timezone to print the current time of\n"},
    {"localdatetime", "'localdatetime': Print the local time and date\nUse: 'localdatetime'\n"},
    {"cpufeatures", "'cpufeatures': Print cpu support for key features like mmx, sse, avx, etc\nUse: 'cpufeatures'\n"},
    {"sleep", "'sleep': Causes the system to sleep for the seconds specified\nUse: 'sleep' [seconds]\n'seconds': Number of seconds for the system to sleep\n"},
    {"test", "'test': Throws the provided exception\nUse: 'test [exception]'\n'exception': Type of exception to be thrown\nAvailable exceptions:\ndiv-by-zero\ninvalid-opcode\n"},
    {"clear", "'clear': Clears the current console\nUse: 'clear'\n"}
//    ,{"quadratic", "'quadratic': Calculates the roots of a quadratic ecuation\nUse: 'quadratic [a] [b] [c]'\n'a': Quadratic coeficient\n'b': Lineal coeficient\n'c': Independent coeficient\n"}
};

static void helpHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if (paramAmount > 1){
        printErr("Too many parameters for command 'help'\nUse: help [command]");
    }
    if (paramAmount == 0){
        printf("Available Commands:\nhelp [command]\necho [message]\nechofloat [precision] [number]\ninforeg\nprintmem [pointer]\ndatetime [timezone]\nlocaldatetime\ncpufeatures\nsleep [seconds]\ntest [exception]\nclear\n");//quadratic [a] [b] [c]\n");
    }
    for(int i=0 ; i < commandAmount ; i++){
        if(strcmp(help_messages[i].name, params[0]) == 0){
            printf(help_messages[i].help_message);
        }
    }
}

static void localDateTimeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'localdatetime'");
        return;
    }
    writeDateTime(LOCAL_UTC);
}

static void cpufeaturesHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 0){
        printErr("Too many parameters for command 'cpufeatures'");
        return;
    }
    writeCpuFeatures();
}

static void sleepHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 1){
        printErr("Missing parameter for command 'sleep'");
        return;
    }
    if(paramAmount > 1){
        printErr("Too many parameters for command 'sleep'");
        return;
    }
    long seconds = strToNumPos(params[0]);
    if(seconds > 0)
        sleep(seconds);
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

static commandStruct commands[] = {
    {"help", &helpHandler},
    {"echo", &echoHandler},
    {"echofloat", &echofloatHandler},
    {"inforeg", &inforegHandler},
    {"printmem", &printmemHandler},
    {"datetime", &datetimeHandler},
    {"localdatetime", &localDateTimeHandler},
    {"cpufeatures", &cpufeaturesHandler},
    {"sleep", &sleepHandler},
    {"test", &testHandler},
    {"clear", &clearHandler}
//    ,{"quadratic", &quadraticHandler}
};

static int getCommandAndParams(char* command, char params[][MAX_PARAMETER_LENGTH], char* input){
    int inputIdx, j=0, paramIdx=0;
    for(inputIdx=0 ; input[inputIdx] != ' ' && input[inputIdx] != '\n' && input[inputIdx] != 0 ; inputIdx++){
        if (inputIdx < MAX_COMMAND_LENGTH)
            command[inputIdx] = input[inputIdx];
    }
    command[inputIdx] = 0;
    if(input[inputIdx] != 0 && input[inputIdx] != '\n'){
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