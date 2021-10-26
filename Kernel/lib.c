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
	char *pos = (char*)startPos;
	for(size_t i=0 ; i < count ; i++){
		target[i] = pos[i];
	}
}

size_t strlen(char *string){
    size_t c = 0;
    for(size_t i=0 ; string[i] != 0 ; i++){
        c++;
    }
    return c;
}

size_t totalStrlen(int dim, char* strArr[]){
	size_t c = 0;
	for(size_t i=0 ; i < dim ; i++){
		c += strlen(strArr[i]);
	}
	return c;
}

static int digitToStr(int num, int base){
	return base > 10 && num > 9 ? num - 10 + 'A' : num + '0';
}

// Transforms an integer to string and stores it on target. Returns final string length
size_t numToStr(long value, char* target, size_t base){
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
void numToStrSized(size_t value, char* target, size_t base, size_t length){
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

static bool hasFeature(uint32_t features, int feature){
	return (features >> feature) & 1;
}

static void fillCommonFeatures(CommonFeatures* features, uint32_t featuresEDX, uint32_t featuresECX){
	features -> mmx = hasFeature(featuresEDX, mmx);
	features -> sse = hasFeature(featuresEDX, sse);
	features -> sse2 = hasFeature(featuresEDX, sse2);
	features -> sse3 = hasFeature(featuresECX, sse3);
	features -> sse41 = hasFeature(featuresECX, sse41);
	features -> sse42 = hasFeature(featuresECX, sse42);
	features -> aes = hasFeature(featuresECX, aes);
	features -> pclmulqdq = hasFeature(featuresECX, pclmulqdq);
	features -> avx = hasFeature(featuresECX, avx);
	features -> f16c = hasFeature(featuresECX, f16c);
	features -> fma = hasFeature(featuresECX, fma);
}

static void fillExtendedFeatures(ExtendedFeatures* features, uint32_t featuresEBX, uint32_t featuresECX){
	features -> vaes = hasFeature(featuresECX, vaes);
	features -> vpclmulqdq = hasFeature(featuresECX, vpclmulqdq);
	features -> avx2 = hasFeature(featuresEBX, avx2);
}

bool getCpuFeatures(CommonFeatures* commonFeatures, ExtendedFeatures* extendedFeatures){
	if(!getCpuIdSupport()){
		return 0;
	}
	uint32_t featuresEDX = getCpuFeaturesEDXRaw();
	uint32_t featuresECX = getCpuFeaturesECXRaw();
	uint32_t extendedFeaturesEBX = getCpuExtendedFeaturesEBXRaw();
	uint32_t extendedFeaturesECX = getCpuExtendedFeaturesECXRaw();
	fillCommonFeatures(commonFeatures, featuresEDX, featuresECX);
	fillExtendedFeatures(extendedFeatures, extendedFeaturesEBX, extendedFeaturesECX);
	return 1;
}