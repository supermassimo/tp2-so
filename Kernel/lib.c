#include <lib.h>

typedef enum cpuFeaturesEDX{
	fpu, vme, de, pse, tsc, msr, pae, mce, cx8, apic,
	sep=11, mtrr, pge, mca, cmov, pat, pse36, psn, clfsh,
	ds=21, acpi, mmx, fxsr, sse, sse2, ss, htt, tm, ia64, pbe
} cpuFeaturesEDX;

typedef enum cpuFeaturesECX{
	sse3, pclmulqdq, dtes64, monitor, dscpl, vmx, smx, est, 
	tm2, ssse3, cnxtid, sdbg, fma, cx16, xtpr, pdcm,
	pcid=17, dca, sse41, sse42, x2apic, movbe, popcnt, tscdeadline,
	aes, xsave, osxsave, avx, f16c, rdrnd, hypervisor
} cpuFeaturesECX;

typedef enum cpuExtendedFeaturesEBX{
	avx2=5
} cpuExtendedFeaturesEBX;

typedef enum cpuExtendedFeaturesECX{
	vaes=9, vpclmulqdq=10
} cpuExtendedFeaturesECX;

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
int numToStr(size_t value, char* target, uint8_t base){
	int digit;
	int sign = 1;		// 0 if negative, 1 if positive
	int i = -1, j = 0;
	char aux[100];
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
void numToStrSized(size_t value, char* target, uint8_t base, int length){
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


// TODO: Move printRegistries() to userland if necessary
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

static int hasFeature(uint32_t features, int feature){
	return (features >> feature) & 1;
}

void printCpuFeatures(){
	char* auxMsg;
	uint32_t featuresEDX = getCpuFeaturesEDXRaw();
	uint32_t featuresECX = getCpuFeaturesECXRaw();
	uint32_t extendedFeaturesEBX = getCpuExtendedFeaturesEBXRaw();
	uint32_t extendedFeaturesECX = getCpuExtendedFeaturesECXRaw();
	// TODO: Add cpuid support, mx support
	print("sse_support: ");
	auxMsg = (hasFeature(featuresEDX, sse))? "Yes" : "No";
	println(auxMsg);
	print("sse2_support: ");
	auxMsg = (hasFeature(featuresEDX, sse2))? "Yes" : "No";
	println(auxMsg);
	print("sse3_support: ");
	auxMsg = (hasFeature(featuresECX, sse3))? "Yes" : "No";
	println(auxMsg);
	print("sse41_support: ");
	auxMsg = (hasFeature(featuresECX, sse41))? "Yes" : "No";
	println(auxMsg);
	print("sse42_support: ");
	auxMsg = (hasFeature(featuresECX, sse42))? "Yes" : "No";
	println(auxMsg);
	print("aesni_support: ");
	auxMsg = (hasFeature(featuresECX, aes))? "Yes" : "No";
	println(auxMsg);
	print("pclmulqdq_support: ");
	auxMsg = (hasFeature(featuresECX, pclmulqdq))? "Yes" : "No";
	println(auxMsg);
	print("avx_support: ");
	auxMsg = (hasFeature(featuresECX, avx))? "Yes" : "No";
	println(auxMsg);
	print("vaesni_support: ");
	auxMsg = (hasFeature(extendedFeaturesECX, vaes))? "Yes" : "No";
	println(auxMsg);
	print("vpclmulqdq_support: ");
	auxMsg = (hasFeature(extendedFeaturesECX, vpclmulqdq))? "Yes" : "No";
	println(auxMsg);
	print("f16c_support: ");
	auxMsg = (hasFeature(featuresECX, f16c))? "Yes" : "No";
	println(auxMsg);
	print("fma_support: ");
	auxMsg = (hasFeature(featuresECX, fma))? "Yes" : "No";
	println(auxMsg);
	print("avx2_support: ");
	auxMsg = (hasFeature(extendedFeaturesEBX, avx2))? "Yes" : "No";
	println(auxMsg);
}