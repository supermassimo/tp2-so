#include <time.h>
#include <stdint.h>

static void irqTimerTick();
static void irqKeyboardRead();
static void irqSysCall();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			irqTimerTick();
			break;
		case 1:
			irqKeyboardRead();
			break;
		case 6:
			irqSysCall();
			break;
	}
	return;
}

void irqTimerTick() {
	timer_handler();
}

void irqKeyboardRead(){
	keyboardIntHandler();
}

void irqSysCall(){
	sysCallHandler();
}