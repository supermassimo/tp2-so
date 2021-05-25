#include <stdint.h>
#include <string.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <lib.h>
#include <rtc.h>
#include <console.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

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

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{
	// int key;
	// char hours[3], seconds[3], minutes[3];
	// char day[3], month[3], year[5];
	// Time dayTime;
	// Date date;
	// clearScreen();
	// printTime(-3);
	// getTime(&dayTime, -3);
	// numToStr(dayTime.hours, hours, 10);
	// numToStr(dayTime.minutes, minutes, 10);
	// numToStr(dayTime.seconds, seconds, 10);
	// print(hours);
	// print(":");
	// print(minutes);
	// print(":");
	// println(seconds);
	// printDate();
	// getDate(&date);
	// numToStr(date.day, day, 10);
	// numToStr(date.month, month, 10);
	// numToStr(date.year, year, 10);
	// print(day);
	// print("/");
	// print(month);
	// print("/");
	// println(year);
	// setBackColor(Green);
	// setForeColor(Black);
	// println("Esperando tecla...");
	// while(1){
	// 	printChar(pollKey());
	// }
	// return 0;
	// ncPrint("[Kernel Main]");
	// ncNewline();
	// ncPrint("  Sample code module at 0x");
	// ncPrintHex((uint64_t)sampleCodeModuleAddress);
	// ncNewline();
	// ncPrint("  Calling the sample code module returned: ");
	// ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	// ncNewline();
	// ncNewline();

	// ncPrint("  Sample data module at 0x");
	// ncPrintHex((uint64_t)sampleDataModuleAddress);
	// ncNewline();
	// ncPrint("  Sample data module contents: ");
	// ncPrint((char*)sampleDataModuleAddress);
	// ncNewline();

	// ncPrint("[Finished]");
	clearScreen();

	load_idt();

	/* getBufferContent() test */
	char buf[getBufferSize()];

	while(1){
		if(peekLastKey() == '\n'){
			getBufferContent(buf);
			print(buf);
		}
	}

	// int segundos = 0;
	// char sec[12];

	// while(1){
	// 	if(seconds_elapsed() - segundos > 0){
	// 		numToStr(segundos, sec, 10);
	// 		print("Pasaron ");
	// 		print(sec);
	// 		println(" segundos");
	// 		segundos++;
	// 	}
	// }

	return 0;
}

