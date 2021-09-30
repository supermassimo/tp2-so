#include "./include/mystdio.h"
#include "./include/mystdlib.h"

#define STD_OUT 0
#define STD_ERR 1

typedef unsigned char uint8_t;

extern void write(int output, const char* buffer, size_t buffer_size);
extern void readInput(char* buffer, size_t buffer_size);
extern void swapConsole();
extern void delKey();

void printErr(const char* string){
	write(STD_ERR, string, strlen(string));
}

void printf(const char* string){
    write(STD_OUT, string, strlen(string));
}

void printInt (long num, size_t string_size, uint8_t base){
    char array[string_size];
    int final_size = numToStr(num, array, base);
    char final_string[final_size+1];
    for (int i=0; i<=final_size; i++){
        final_string[i] = array[i];
    }
    write (0, final_string, string_size);
}

void printFloat (float num, size_t string_size, size_t precision, uint8_t base){
    char array[string_size];
    int final_size = floatToStr(num, array, precision, base);
    char final_string[final_size+1];
    for (int i=0; i<=final_size; i++){
        final_string[i] = array[i];
    }
    write (0, final_string, string_size);
}

static void printRegistry(const char* msg, uint64_t value){
	printf(msg);
	printInt(value, 16, 16);
	printf("\n");
}

void printRegistries(const Registries regs){
	printRegistry("RAX: ", regs.RAX);
	printRegistry("RBX: ", regs.RBX);
	printRegistry("RCX: ", regs.RCX);
	printRegistry("RDX: ", regs.RDX);
	printRegistry("RDI: ", regs.RDI);
	printRegistry("RSI: ", regs.RSI);
	printRegistry("R8: " , regs.R8 );
	printRegistry("R9: " , regs.R9 );
	printRegistry("R10: ", regs.R10);
	printRegistry("R11: ", regs.R11);
	printRegistry("R12: ", regs.R12);
	printRegistry("R13: ", regs.R13);
	printRegistry("R14: ", regs.R14);
	printRegistry("R15: ", regs.R15);
	printRegistry("RBP: ", regs.RBP);
	printRegistry("RSP: ", regs.RSP);
	printRegistry("RIP: ", regs.RIP);
}

void printFeatures(const CommonFeatures commonFeatures, const ExtendedFeatures extendedFeatures){
	char* auxMsg;
	printf("mmx_support: ");
	auxMsg = commonFeatures.mmx ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("sse_support: ");
	auxMsg = commonFeatures.sse ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("sse2_support: ");
	auxMsg = commonFeatures.sse2 ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("sse3_support: ");
	auxMsg = commonFeatures.sse3 ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("sse41_support: ");
	auxMsg = commonFeatures.sse41 ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("sse42_support: ");
	auxMsg = commonFeatures.sse42 ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("aesni_support: ");
	auxMsg = commonFeatures.aes ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("pclmulqdq_support: ");
	auxMsg = commonFeatures.pclmulqdq ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("avx_support: ");
	auxMsg = commonFeatures.avx ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("vaesni_support: ");
	auxMsg = extendedFeatures.vaes ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("vpclmulqdq_support: ");
	auxMsg = extendedFeatures.vpclmulqdq ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("f16c_support: ");
	auxMsg = commonFeatures.f16c ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("fma_support: ");
	auxMsg = commonFeatures.fma ? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
	printf("avx2_support: ");
	auxMsg = extendedFeatures.avx2? "Yes" : "No";
	printf(auxMsg);
	printf("\n");
}

void printMemContent(uint64_t startPos, uint8_t* memContent, size_t amount){
	char numStr[3];
	for(size_t i=0 ; i < amount ; i++){
		if(i%8 == 0){
			printInt(startPos+i, 2, 16);
			printf("h: ");
		}
		// printInt(memContent[i], 2, 16);
		numToStrSized((long)*(memContent+i), numStr, 16, 2);
		printf(numStr);
		printf(" ");
		if((i+1)%8 == 0)
			printf("\n");
	}
}

// Prints daytime in format HH:MM:SS (24hs)
void printTime(Time dayTime){
	char hours[3], minutes[3], seconds[3];
	numToStrSized(dayTime.hours, hours, 10, 2);
	numToStrSized(dayTime.minutes, minutes, 10, 2);
	numToStrSized(dayTime.seconds, seconds, 10, 2);
	printf("Time: ");
	printf(hours);
	printf(":");
	printf(minutes);
	printf(":");
	printf(seconds);
	printf("\n");
}

// Prints date in format DD/MM/YYYY
void printDate(Date date){
	char day[3], month[3], year[5];
	numToStrSized(date.day, day, 10, 2);
	numToStrSized(date.month, month, 10, 2);
	numToStrSized(date.year, year, 10, 4);
	printf("Date: ");
	printf(day);
	printf("/");
	printf(month);
	printf("/");
	printf(year);
	printf("\n");
}

// Prints date and time in formats DD/MM/YYYY and HH:MM:SS (24hs) respectively
void printDateTime(const Date date, const Time time, const int utc){
	printf("UTC: ");
	printInt(utc, 2, 10);
	printf("\n");
	printDate(date);
	printTime(time);
}

void printMemInfo(MemoryInfo meminfo, char* option){
	int iskB;
	if((iskB = strcmp(option, "-k")) == 0){
		meminfo.totalMemory /= 1000;
		meminfo.occupiedMemory /= 1000;
	}
	printf("MemTotal: ");
	printInt(meminfo.totalMemory, 30, 10);	// 30 is more than enough for 64 bytes
	printf(iskB?" B" : " kB");
	printf("\n");
	printf("MemOccupied: ");
	printInt(meminfo.occupiedMemory, 30, 10);	// 30 is more than enough for 64 bytes
	printf(iskB?" B" : " kB");
	printf("\n");
}

void scanf (char* string, size_t string_size){
    readInput (string, string_size);
}

void consoleSwap(){
	swapConsole();
}

void deleteKey(){
	delKey();
}