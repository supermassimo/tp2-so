#include <stdef.h>

extern write(int output, const char* buffer, size_t buffer_size);
extern read(char* buffer, size_t buffer_size);

static int digitToStr(int num, int base){
	return base > 10 ? num - 10 + 'A' : num + '0';
}
// Transforms an integer to string and stores it on target. Returns final string length
static int numToStr(size_t value, char* target, uint8_t base){
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

void printf (const char* string, size_t string_size){
    write (0, string, string_size);
}

void printInt (int num, size_t string_size, uint8_t base){
    char array[string_size];
    int final_size = numToStr(num, array, base);
    char final_string[final_size+1];
    for (int i=0; i<=final_size; i++){
        final_size[i] = array[i];
    }
    write (0, string, string_size);
}

void scanf (char* string, size_t string_size){
    read (string, string_size);
}
