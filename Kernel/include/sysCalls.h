#ifndef SYSCALLHANDLER_H_
#define SYSCALLHANDLER_H_

void sysCallHandler();
void readInput(char* buffer, size_t buffer_size);
void write(int output, const char* buffer, size_t buffer_size);

#endif