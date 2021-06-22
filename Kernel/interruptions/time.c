#include <time.h>
#include <keyboard.h>

extern void _hlt();

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

void timerTickSleep(long seconds){
	long currentSeconds = seconds_elapsed();
	long finalSeconds = currentSeconds + seconds;
	disableKeyboardInt();
	while(currentSeconds < finalSeconds){
		_hlt();
		currentSeconds = seconds_elapsed();
	}
	enableKeyboardInt();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
