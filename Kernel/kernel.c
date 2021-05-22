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

/**************************************************************************************************/

const int keyTable[] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', 168,			// 1:ESC
	'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '´', '+',		// 14:BACKSPACE
	'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 164, '{', '|',		// 29:CTRL
	15, '}', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0, '*', 0, ' ',	// 42:SHIFT(izq), 54:SHIFT(der), 55:*, 56:ALT/ALTGR, 57:SPACEBAR. 53 tb es '/'
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7',                          		// 58:BLOQ MAYUS, 59-68:F1-F10, 69:BLOQ NUM, 70:BLOQ DESPL, 71:INICIO y 7 (tecl num)
	'↑', '9', '-', '←', '5', '→', '+', '1', '↓', '3', 0, 0, 0, 0,				// 72:↑ y 8(tecl num), 73:REPAG y 9 (tecl num), 74:- (tecl num), 75:← y 4 (tecl num), 76:5 (tecl num), 77:→ y 6 (tecl num), 78:+ (tecl num), 79:FIN y 1 (tecl num), 80:↓ y 2 (tecl num), 81:AVPAG y 3 (tecl num), 82:INS, 83:SUPR, 84:?, 85:?
	0, 0, 0, 0, 0, 0, 0, 0														// 86:?, 87:F11, 88:F12, 89:?, 90:?, 91:TECLA WINDOWS, 92:?, 93:CLICK IZQUIERDO
};

/**************************************************************************************************/

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
	int key;
	char hours[3], seconds[3], minutes[3];
	char day[3], month[3], year[5];
	Time dayTime;
	Date date;
	clearScreen();
	printTime(-3);
	getTime(&dayTime, -3);
	numToStr(dayTime.hours, hours, 10);
	numToStr(dayTime.minutes, minutes, 10);
	numToStr(dayTime.seconds, seconds, 10);
	print(hours);
	print(":");
	print(minutes);
	print(":");
	println(seconds);
	printDate();
	getDate(&date);
	numToStr(date.day, day, 10);
	numToStr(date.month, month, 10);
	numToStr(date.year, year, 10);
	print(day);
	print("/");
	print(month);
	print("/");
	println(year);
	// setBackColor(Green);
	// setForeColor(Black);
	println("Esperando tecla...");
	while(1){
		key = pollKey();
		if(key < 128){
			key = keyTable[key];
			printChar(key);
		}
	}
	return 0;
	/*
	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	ncPrint("[Finished]");
	return 0;
	*/
}

