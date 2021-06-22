#include "include/mystdlib.h"
#include "include/mystdio.h"

int strcmp(char* s1, char* s2){
    int c, i=-1;
    do{
        i++;
        c = s1[i] - s2[i];
    } while(s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i]);
    return c;
}

size_t strlen(char *string){
    size_t c=0;
    for(size_t i=0 ; string[i] != 0 ; i++){
        c++;
    }
    return c;
}


// Concatenates all the strings sent and returns output string length
size_t concatStrings(char strings[][MAX_PARAMETER_LENGTH], size_t stringAmount, char* output){
    size_t outputIdx=0, strIdx=0, i=0;
    while(strIdx < stringAmount){
        if(strings[strIdx][i] == 0){         
            if(strIdx < stringAmount - 1)
                output[outputIdx++] = ' ';
            else
                output[outputIdx] = 0;
            strIdx++;
            i = 0;
        }
        else {
            output[outputIdx++] = strings[strIdx][i++];
        }
    }
    return outputIdx;
}

static int isDigit(int digit){
    if(digit < 0 || digit > 9){
        printErr("String sent is not a number");
        return 0;
    }
    return 1;
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
void numToStrSized(long value, char* target, size_t base, size_t length){
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

// Transforms string to unsigned number. Returns -1 if number sent was negative
long strToNumPos(char* string){
    size_t num = 0, digit, i=0;
    size_t length = strlen(string);
    if(string[0] == '-'){
        printErr("String sent must be positive");
        return -1;
    }
    while(i < length){
        num *= 10;
        digit = string[i] - '0';
        if(!isDigit(digit))
            break;
        num += digit;
        i++;
    }
    return num;
}

long strToNum(char* string){
    int isNegative=0, digit;
    long num = 0;
    size_t i=0, length = strlen(string);
    if(string[0] == '-'){
        isNegative = 1;
        i++;
    }
    while(i < length){
        num *= 10;
        digit = string[i] - '0';
        if(!isDigit(digit))
            break;
        num += digit;
        i++;
    }
    if(isNegative)
        num *= -1;
    return num;
}

//precision es el numero de caracteres despues de la coma
size_t floatToStr(float value, char* target, size_t precision, size_t base){
    int digit;
    int j = numToStr((int)value, target, base);

	if(value < 0)
		value *= -1;
    float value2 = value - ((int)value);

    target[j++] = '.';
    if (precision == 0)
        target[j++] = '0';
    else {
        for (int p=0; p<precision; p++){
            value2 *= base;
		    digit = (int)value2;
		    target[j++] = digitToStr(digit, base);
            value2 -= digit;
        }
    }

	target[j] = 0;
	return j;
}

int strToFloat(char* string, float* target){
    int isNegative=0, digit;
    float num=0, fraction=0;
    size_t i=0, length = strlen(string);
    if(string[0] == '-'){
        isNegative = 1;
        i++;
    }
    int afterPoint = 0;
    long offset = 10;
    while(i < length){
        if (string[i] == '.'){
            if (afterPoint){
                printErr("String sent is not valid");
                return -1;
            }
            afterPoint = 1;
        } else {
            digit = string[i] - '0';
                if(!isDigit(digit))
                    return -1;
            if (afterPoint){
                fraction += digit * (1.0/(offset));
                offset*=10;
            } else {
                num *= 10;
                num += digit;
            }
        }
        i++;
    }
    num += fraction;

    if(isNegative)
        num *= -1;

    *target = num;
    
    return 0;
}