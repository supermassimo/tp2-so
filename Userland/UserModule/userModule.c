/* userModule.c */
#include "./standard_library/include/mystdio.h"
#include "./standard_library/include/mystdlib.h"
#include "./standard_library/include/commands.h"

// char * v = (char*)0xB8000 + 79 * 2;

// static int var1 = 0;
// static int var2 = 0;

int main() {
	char input[256];
	// writeRegistries();
	// printf("beep boop printing from userland", 33);
	while(1){
	 	scanf(input, 255);
		if(strcmp(input, "") != 0)
			commandHandler(input);
	};
	//All the following code may be removed 
	// *v = 'X';
	// *(v+1) = 0x74;

	// //Test if BSS is properly set up
	// if (var1 == 0 && var2 == 0)
	// 	return 0xDEADC0DE;

	return 0xDEADBEEF;
}