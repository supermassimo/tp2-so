// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <keyboard.h>
#include <stdint.h>

static void irqTimerTick();
static void irqKeyboardRead();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			irqTimerTick();
			break;
		case 1:
			irqKeyboardRead();
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