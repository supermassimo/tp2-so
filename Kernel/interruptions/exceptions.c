#include <console.h>

static void zero_division();
static void invalid_op();
static void exceptionReloadPointer();
extern void loader();

void exceptionDispatcher(int exception) {
	switch (exception) {
		case 0:
			zero_division();
			break;
		case 6:
			invalid_op();
			break;
	}
	return;
}

static void zero_division() {
	printErr("ERROR: Cannot divide by zero");
	printRegistries();

	// exceptionReloadPointer();
}

static void invalid_op() {
	printErr("ERROR: Invalid opcode");
	printRegistries();
	
	// exceptionReloadPointer();
}

static void exceptionReloadPointer(){
	main();
}