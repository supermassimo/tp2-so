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

static int initialized = 0;

int main() {
	if (initialized != 1){ //initialization routine for first time, can put anything here that needs to only run once on startup
		consoleSwap();
		printf("> ");
		consoleSwap();
		printf("> ");
		initialized = 1;
	}
	waitForInput();
	return 0xDEADBEEF;
}