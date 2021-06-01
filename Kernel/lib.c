#include <lib.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

// Fills target array with #count bytes of memory starting from startPos
void getMemContent(uint64_t startPos, uint8_t* target, size_t count){
	int8_t *pos = startPos;
	for(int i=0 ; i < count ; i++){
		target[i] = pos[i];
	}
}

static int digitToStr(int num, int base){
	return base > 10 ? num - 10 + 'A' : num + '0';
}

// Transforms an integer to string and stores it on target. Returns final string length
int numToStr(int value, char* target, uint8_t base){
	int digit;
	int sign = 1;		// 0 if negative, 1 if positive
	int i = -1, j = 0;
	char aux[12];
	if(value < 0){
		sign = 0;
		value *= -1;
	}
	do{
		i++;
		digit = value % base;
		aux[i] = digitToStr(digit, base);
		value /= base;
	} while(value > 0);
	if(!sign){
		target[j++] = '-';
	}
	while(i > -1){
		target[j++] = aux[i--];
	}
	target[j] = 0;
	return j;
}

// Left-fills the number with zeros until required length is met
void numToStrSized(int value, char* target, uint8_t base, int length){
	int currLen = numToStr(value, target, base);
	int difference = length - currLen;
	if(difference > 0){
		for(int i=length-1 ; i >= 0 ; i--){
			if(i >= difference)
				target[i] = target[i-difference];
			else
				target[i] = '0';
		}
		target[length] = 0;
	}
}

void printRegistries(){
	Registries regs;
	getRegistries(&regs);
	print("RAX: ");
	printInt(regs.rax, 10);
	newLine();
	print("RBX: ");
	printInt(regs.rbx, 10);
	newLine();
	print("RCX: ");
	printInt(regs.rcx, 10);
	newLine();
	print("RDX: ");
	printInt(regs.rdx, 10);
	newLine();
	print("RBP: ");
	printInt(regs.rbp, 10);
	newLine();
	print("RDI: ");
	printInt(regs.rdi, 10);
	newLine();
	print("RSI: ");
	printInt(regs.rsi, 10);
	newLine();
	print("R8: ");
	printInt(regs.r8, 10);
	newLine();
	print("R9: ");
	printInt(regs.r9, 10);
	newLine();
	print("R10: ");
	printInt(regs.r10, 10);
	newLine();
	print("R11: ");
	printInt(regs.r11, 10);
	newLine();
	print("R12: ");
	printInt(regs.r12, 10);
	newLine();
	print("R13: ");
	printInt(regs.r13, 10);
	newLine();
	print("R14: ");
	printInt(regs.r14, 10);
	newLine();
	print("R15: ");
	printInt(regs.r15, 10);
	newLine();
}

void getRegistries (Registries* destination){
	destination -> rax = getRAX();
	destination -> rbx = getRBX();
	destination -> rcx = getRCX();
	destination -> rdx = getRDX();
	destination -> rbp = getRBP();
	destination -> rdi = getRDI();
	destination -> rsi = getRSI();
	destination -> r8 = getR8();
	destination -> r9 = getR9();
	destination -> r10 = getR10();
	destination -> r11 = getR11();
	destination -> r12 = getR12();
	destination -> r13 = getR13();
	destination -> r14 = getR14();
	destination -> r15 = getR15();
}