// #include "./include/commands.h"
// #include "./include/mystdio.h"
// #include "./include/mystdlib.h"

// /*              TEST MM HEADERS             */
// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// #include "./include/test_util.h"
// /*******************************************/



// int main(){

//     char input[120];
// 	while(1){
// 	 	scanf(input, 120);
// 		if(strlen(input) <= 1){
// 			if(strcmp(input, "\t") == 0)
// 				consoleSwap();
// 			if(strcmp(input, "\b") == 0)
// 				deleteKey();
// 		} else if(strcmp(input, "") != 0)
// 			commandHandler(input);
// 	};

//     while (1) {
// 		/* Print the command prompt */
// 		printf("$> ");
// 		//fflush(NULL);
 
// 		/* Read a command line */
// 		if (!fgets(line, 1024, stdin)) 
// 			return 0;
 
// 		int input = 0;
// 		int first = 1;
 
// 		char* cmd = line;
// 		char* next = strchr(cmd, '|'); /* Find first '|' */
 
// 		while (next != NULL) {
// 			/* 'next' points to '|' */
// 			*next = '\0';
// 			input = run(cmd, input, first, 0);
 
// 			cmd = next + 1;
// 			next = strchr(cmd, '|'); /* Find next '|' */
// 			first = 0;
// 		}
// 		input = run(cmd, input, first, 1);
// 		cleanup(n);
// 		n = 0;
// 	}

//     return 0;
// }

// static int getCommandAndParams(char* command, char params[][MAX_PARAMETER_LENGTH], char* input){
//     int inputIdx, paramIdx=0;
//     for(inputIdx=0 ; input[inputIdx] != ' ' && !isEnd(input[inputIdx]); inputIdx++){
//         if (inputIdx < MAX_COMMAND_LENGTH)
//             command[inputIdx] = input[inputIdx];
//     }
//     command[inputIdx] = 0;
//     if(!isEnd(input[inputIdx])){
//         int j=0;
//         inputIdx++;
//         while(!isEnd(input[inputIdx])){
//             if(input[inputIdx] == ' '){
//                 params[paramIdx++][j] = 0;
//                 j = 0;
//                 inputIdx++;
//                 continue;
//             }
//             params[paramIdx][j++] = input[inputIdx++];
//         }
//         params[paramIdx++][j] = 0;
//     }
//     return paramIdx;
// }

// void commandHandler(char* string){
//     char commandName[MAX_COMMAND_LENGTH+1] = "";
//     char params[MAX_PARAMETER_AMOUNT][MAX_PARAMETER_LENGTH];
//     int paramAmount = getCommandAndParams(commandName, params, string);
//     for(int i=0 ; i < commandAmount ; i++){
//         if(strcmp(commands[i].name, commandName) == 0){
//             setIdle(0);
//             ((void(*)(char[][MAX_PARAMETER_LENGTH], size_t))commands[i].handler)(params, paramAmount);
//             printf("> ");
//             setIdle(1);
//             return;
//         }
//     }
//     printErr("Unknown command. Use 'help' for a list of commands");
//     printf("> ");
// }