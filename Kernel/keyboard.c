// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard.h>
#include <console.h>
#include <standardIn.h>
#include <dualDisplayManager.h>
#include <lib.h>                        // For testing purposes. DO NOT DELETE
#include <sysCalls.h>                   // For testing purposes. DO NOT DELETE

#define KEYTABLE_SIZE 94

extern void captureRegistries();

static char keyboard_buffer_0[BUFFER_SIZE];
static unsigned int nextToStore_0 = 0;

static char keyboard_buffer_1[BUFFER_SIZE];
static unsigned int nextToStore_1 = 0;

static int isEnabled = 1;
static int isShiftPressed = 0;

static const int keyTable[KEYTABLE_SIZE] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', 168,			// 1:ESC
	'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0, '+',		// 14:BACKSPACE
	'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 164, '{', '~',		// 29:CTRL
	15, '}', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0, '*', 0, ' ',	// 42:SHIFT(izq), 54:SHIFT(der), 55:*, 56:ALT/ALTGR, 57:SPACEBAR. 53 tb es '/'
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7',                          		// 58:BLOQ MAYUS, 59-68:F1-F10, 69:BLOQ NUM, 70:BLOQ DESPL, 71:INICIO y 7 (tecl num)
	0, '9', '-', 0, '5', 0, '+', '1', 0, '3', 0, 0, 0, 0,				        // 8 (tecl num), 73:REPAG, 9 (tecl num), 74:- (tecl num), 4 (tecl num), 76:5 (tecl num), 6 (tecl num), 78:+ (tecl num), 79:FIN, 1 (tecl num), 2 (tecl num), 81:AVPAG, 3 (tecl num), 82:INS, 83:SUPR, 84:?, 85:?
	0, 0, 0, 0, 0, 0, 0, 0														// 86:?, 87:F11, 88:F12, 89:?, 90:?, 91:TECLA WINDOWS, 92:?, 93:CLICK IZQUIERDO
};

static const int keyTableShift[KEYTABLE_SIZE] = {
	0, 27, '!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '?', '¡', '\b', 	// 1:ESC
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0, '*',		        // 14:BACKSPACE
	'\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Ñ', '[', '|',		// 29:CTRL
	0, ']', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ':', ':', '_', 0, 0, 0, ' ',	    // 42:SHIFT(izq), 54:SHIFT(der), 55:|, 56:ALT/ALTGR, 57:SPACEBAR. 53 tb es '/'
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7',                          		// 58:BLOQ MAYUS, 59-68:F1-F10, 69:BLOQ NUM, 70:BLOQ DESPL, 71:INICIO y 7 (tecl num)
	0, '9', '-', 0, '5', 0, '+', '1', 0, '3', 0, 0, 0, 0,				        // 8 (tecl num), 73:REPAG, 9 (tecl num), 74:- (tecl num), 4 (tecl num), 76:5 (tecl num), 6 (tecl num), 78:+ (tecl num), 79:FIN, 1 (tecl num), 2 (tecl num), 81:AVPAG, 3 (tecl num), 82:INS, 83:SUPR, 84:?, 85:?
	0, 0, 0, 0, 0, 0, 0, 0														// 86:?, 87:F11, 88:F12, 89:?, 90:?, 91:TECLA WINDOWS, 92:?, 93:CLICK IZQUIERDO
};

extern uint8_t pollKeyRaw();

void enableKeyboardInt(){
    isEnabled = 1;
}

void disableKeyboardInt(){
    isEnabled = 0;
}

static int keyboardBufferIsFull(int offset){
    if (getCurrentDisplay())
        return nextToStore_1 + offset >= BUFFER_SIZE;
    else
        return nextToStore_0 + offset >= BUFFER_SIZE;
}

int keyboardBufferIsEmpty(){
    if (getCurrentDisplay())
        return nextToStore_1 == 0;
    else
        return nextToStore_0 == 0;
}

void emptyKeyboardBuffer(){
    if (getCurrentDisplay())
        nextToStore_1 = 0;
    else
        nextToStore_0 = 0;
}

// Stores a key on the keyboard buffer
static void typeKey(int key){
    if(!keyboardBufferIsFull(0)){
        if (getCurrentDisplay()){
            keyboard_buffer_1[nextToStore_1++] = key;
        } else {
            keyboard_buffer_0[nextToStore_0++] = key;
        }
        printChar(key);
    }
}

void keyboardDeleteKey(){
    if (!keyboardBufferIsEmpty()){
        if (getCurrentDisplay()){
            nextToStore_1--;
        } else {
            nextToStore_0--;
        }
        deleteKeyConsole();
    }
}

void pushKeyboardBuffer(){
    if (getCurrentDisplay()){
        setInputBuffer(keyboard_buffer_1, nextToStore_1);
        nextToStore_1 = 0;
    }else{
        setInputBuffer(keyboard_buffer_0, nextToStore_0);
        nextToStore_0 = 0;
    }
}

void pushSingleKey(int c){
    char str[1];
    str[0] = c;
    setInputBuffer(str, 1);
}

// 0-31, 126 and 127 are reserved ASCII control characters
static int isControlKey(int c){
	return c < 32 || c == 126 || c == 127;
}

// Reads the input
// If there's a key, stores it on buffer/applies action on buffer and returns it
// If there's not a key, returns -1
static int readKey(){
    int key = -1;
    uint8_t keyCode = pollKeyRaw();
    if (keyCode == 42) { isShiftPressed = true; return key; } //42 is shift press code
    if (keyCode == 170) { isShiftPressed = false; return key; } //170 is shift lift code
    // If it´s a MAKE, a key was pressed
    if(keyCode < KEYTABLE_SIZE){        //if(keyCode < 127) on full table (table only contains 94 keys)
        if (isShiftPressed){
            key = keyTableShift[keyCode];
        } else {
            key = keyTable[keyCode];
        }
    }
    return key;
}

void keyboardIntHandler(){
    // char msg[100];                                              // For testing purposes. DO NOT DELETE
    if(isEnabled){
        int key = readKey();
        if (isControlKey(key)){
            if (key == '~'){    // This key must be handled by the kernel as the registry capture has to be done within the same interrupt
                captureRegistries();
            } else if(key == '\n'){
                typeKey(key);
                pushKeyboardBuffer();
            }
            else
                pushSingleKey(key);
        } else if(!keyboardBufferIsFull(1)){
            typeKey(key);
        }
    }
}


void awaitForInstantInput(){
    print("Press any key to continue...");
    while(keyboardBufferIsEmpty());
    emptyKeyboardBuffer();
}