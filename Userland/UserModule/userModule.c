// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* userModule.c */
#include "./standard_library/include/mystdio.h"
#include "./standard_library/include/mystdlib.h"
#include "./standard_library/include/processes.h"

#include "./processes/shell.c"

static int initialized = 0;

int main() {
	if (initialized != 1){ //initialization routine for first time, can put anything here that needs to only run once on startup
		consoleSwap();
		printf("> ");
		consoleSwap();
		printf("> ");
		initialized = 1;
	}

	char* args[0];
	createFullProcess(&shell_init, LOW, 0, args, "shell");
	
	return 0xDEADBEEF;
}