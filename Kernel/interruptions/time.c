// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <keyboard.h>

extern void _hlt();

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

// Previously used to sleep shell. Delete on final version
/*
void timerTickSleep(long seconds){
	long currentSeconds = seconds_elapsed();
	long finalSeconds = currentSeconds + seconds;
	disableKeyboardInt();
	while(currentSeconds < finalSeconds){
		_hlt();
		currentSeconds = seconds_elapsed();
	}
	print("SALGOOO");
	enableKeyboardInt();
}
*/

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
