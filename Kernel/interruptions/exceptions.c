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
	println("Zero div not allowed");
}

static void invalid_op() {
	// Handler para manejar excepcíon
}