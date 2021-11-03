// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <moduleLoader.h>
#include <lib.h>
#include <console.h>
#include <scheduler.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

extern void load_idt();

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
	// char buffer[10];
	void * moduleAddresses[] = {
		userCodeModuleAddress
		,dataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

// Previously used to jump directly to userlands' _start
void loadUserModuleAdress(){
	((EntryPoint)userCodeModuleAddress)();
}

int main()
{
	clearScreen();
	load_idt();

	initializeConsole();

	// loadUserModuleAdress();
	createHaltingProcess();
	createProcess(userCodeModuleAddress, SYSTEM, 0, NULL, "init");
	enableScheduler();

	while(1);

	return 0;
}