#include <stdint.h>

int getBufferSize();
int getKey();
int getBufferContent(unsigned char *target);
int peekLastKey();
void keyboardIntHandler();