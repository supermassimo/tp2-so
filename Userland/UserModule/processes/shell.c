#include "./include/commands.h"
#include "./include/mystdio.h"
#include "./include/mystdlib.h"

/*              TEST MM HEADERS             */
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "./include/test_util.h"
/*******************************************/

int main(){

    char input[120];
	while(1){
	 	scanf(input, 120);
		if(strlen(input) <= 1){
			if(strcmp(input, "\t") == 0)
				consoleSwap();
			if(strcmp(input, "\b") == 0)
				deleteKey();
		} else if(strcmp(input, "") != 0)
			commandHandler(input);
	};

    return 0;
}
