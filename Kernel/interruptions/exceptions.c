// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <console.h>
#include <standardIn.h>

static void zero_division();
static void invalid_op();
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
	print("> ");
	return;
}

static void zero_division() {
	printErr("ERROR: Cannot divide by zero");
}

static void invalid_op() {
	printErr("ERROR: Invalid opcode");
}