#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

int keyboardBufferIsEmpty();
void keyboardIntHandler();
void awaitForInstantInput();
void keyboardDeleteKey();
void pushKeyboardBuffer();
void enableKeyboardInt();
void disableKeyboardInt();

#endif