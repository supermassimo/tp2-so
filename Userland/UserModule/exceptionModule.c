/* exceptionModule.c */
#include "./standard_library/include/mystdio.h"
#include "./standard_library/include/mystdlib.h"
#include "./standard_library/include/commands.h"

extern void restart();
extern void writeRegistries();

// char * v = (char*)0xB8000 + 79 * 2;

// static int var1 = 0;
// static int var2 = 0;

int main2() {
	char input[256];
	writeRegistries();
	printf("Press any key to restart the User Module");
	while(1){
	 	scanf(input, 255);
		if(strcmp(input, "") != 0)
			restart();
	};
	return 0xDEADBEEF;
}