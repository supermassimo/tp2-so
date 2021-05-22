#include <stdint.h>
#include <registryStruct.h>
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

static int digitToStr(int num, int base){
	return base > 10 ? num + 'A' : num + '0';
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

void getRegistries (Registries* destination){
	Registries regStruct;
	regStruct.rax = getRAX();
	regStruct.rbx = getRBX();
	regStruct.rcx = getRCX();
	regStruct.rdx = getRDX();
	regStruct.rbp = getRBP();
	regStruct.rdi = getRDI();
	regStruct.rsi = getRSI();
	regStruct.r8 = getR8();
	regStruct.r9 = getR9();
	regStruct.r10 = getR10();
	regStruct.r11 = getR11();
	regStruct.r12 = getR12();
	regStruct.r13 = getR13();
	regStruct.r14 = getR14();
	regStruct.r15 = getR15();

	destination = regStruct;
}