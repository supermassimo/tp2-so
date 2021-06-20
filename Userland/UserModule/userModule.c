/* userModule.c */
#include "./standard_library/include/mystdio.h"
#include "./standard_library/include/mystdlib.h"
#include "./standard_library/include/commands.h"

void waitForInput(){
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
}

int main() {
	consoleSwap();
	printf("> ");
	consoleSwap();
	printf("> ");
	
	waitForInput();
	return 0xDEADBEEF;
}