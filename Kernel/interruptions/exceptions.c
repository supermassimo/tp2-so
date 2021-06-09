#include <console.h>
#include <standardIn.h>

static void zero_division();
static void invalid_op();
static void exceptionReloadPointer();
extern void loader();

void waitUntilRestart(){
	char keys[BUFFER_SIZE];
	getBufferContent(keys, BUFFER_SIZE);
	print("Press any key to restart system...");
	while(!getBufferContent(keys, BUFFER_SIZE));
}

void exceptionDispatcher(int exception) {
	switch (exception) {
		case 0:
			zero_division();
			break;
		case 6:
			invalid_op();
			break;
	}
	printRegistries();
	return;
}

static void zero_division() {
	printErr("ERROR: Cannot divide by zero");
}

static void invalid_op() {
	printErr("ERROR: Invalid opcode");
}