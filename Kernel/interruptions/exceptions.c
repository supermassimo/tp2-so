#include "../include/console.h"

static void zero_division();
static void invalid_op();

void exceptionDispatcher(int exception) {
	switch (exception) {
		case 0:
			zero_division();
			break;
		case 1:
			invalid_op();
			break;
	}
	return;
}

static void zero_division() {
	println("ERROR: Cannot divide by zero");
}

static void invalid_op() {
	println("ERROR: Invalid opcode");
}