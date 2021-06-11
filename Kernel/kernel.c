#include <stdint.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <lib.h>
#include <rtc.h>
#include <console.h>
#include <sysCalls.h>
#include <standardIn.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const userCodeModuleAddress = (void*)0x400000;
static void * const dataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];
	void * moduleAddresses[] = {
		userCodeModuleAddress
		,dataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

void rebootKernel(){
	loader();
}

void loadUserModuleAdress(){
	((EntryPoint)userCodeModuleAddress)();
}

static void selectMode(){
	println("Select mode to load (T/G)\nT: text mode\nG: graphic mode\n");
	int sel = -1;
	setIdle(0);
	while (sel == -1){
		char string[BUFFER_SIZE];
		if (!bufferIsEmpty()){
			getBufferContent(string, BUFFER_SIZE);
			if (string[0] == 't' || string[0] == 'T')
				sel = 0;
			else if (string[0] == 'g' || string[0] == 'G')
				sel = 1;
			else
				println("invalid key. choose (T/G)\nT: text mode\nG: graphic mode\n");
		}
		if (sel == 1) {
			//would enable graphic mode here
			printErr("Graphic Mode is not available\n");
			sel = -1;
		}
	}
	setIdle(1);
}

int main()
{
	clearScreen();
	load_idt();

	selectMode();
	clearScreen();

	initializeConsole();

	loadUserModuleAdress();
	return 0;
}

