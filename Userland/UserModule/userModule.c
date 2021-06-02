/* userModule.c */
#include "./standard_library/include/mystdio.h"

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {
	char input[255];
	scanf(input, 255);
	char aux = input[10];
	printf("beep boop printing from userland", 33);
	// do{
	// 	scanf(input, 10);
	// }while(input[10] == aux);
	// printf(input, 12);
	//All the following code may be removed 
	// *v = 'X';
	// *(v+1) = 0x74;

	// //Test if BSS is properly set up
	// if (var1 == 0 && var2 == 0)
	// 	return 0xDEADC0DE;

	return 0xDEADBEEF;
}