/* userModule.c */
#include "./standard_library/include/mystdio.h"
#include "./standard_library/include/mystdlib.h"
#include "./standard_library/include/commands.h"
#include "./standard_library/include/consoleStruct.h"

extern void setupConsole(ConsoleParameters* setupValues);

void waitForInput(){
	char input[120];
	while(1){
	 	scanf(input, 120);
		if(strlen(input) <= 1){
			if(strcmp(input, "\t") == 0)
				consoleSwap();
		} else if(strcmp(input, "") != 0)
			commandHandler(input);
	};
}

int main() {
	ConsoleParameters setupValues = {
		'>',
		'|',
		Green,
		Blue,
		Black,
		Black,
		White,
		White,
		DarkGray,
		DarkGray,
		Red,
		Red,
		Brown,
		Brown,
		'~',
		'\n',
		'\b'
	};
	setupConsole(&setupValues);
	waitForInput();
	return 0xDEADBEEF;
}