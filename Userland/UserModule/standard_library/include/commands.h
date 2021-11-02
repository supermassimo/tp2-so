#ifndef COMMANDS_H_
#define COMMANDS_H_

#define MAX_COMMAND_LENGTH 120
#define MAX_PARAMETER_AMOUNT 10
#define MAX_PARAMETER_LENGTH 120

enum CommandState {COMMAND1, PARAM1, COMMAND2, PARAM2, PIPED};

void commandHandler(char* string);

#endif
