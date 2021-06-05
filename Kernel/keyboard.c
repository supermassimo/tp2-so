#include <keyboard.h>
#include <console.h>
#include <standardIn.h>
#include <dualDisplayManager.h>

static unsigned char keyboard_buffer_0[BUFFER_SIZE];
static unsigned int nextToStore_0 = 0;
static unsigned int currentToRead_0 = 0;

static unsigned char keyboard_buffer_1[BUFFER_SIZE];
static unsigned int nextToStore_1 = 0;
static unsigned int currentToRead_1 = 0;

static unsigned char lastKey;               // Testing purposes. Delete when making final code

static const int keyTable[] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', 168,			// 1:ESC
	'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '´', '+',		// 14:BACKSPACE
	'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 164, '{', '|',		// 29:CTRL
	15, '}', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0, '*', 0, ' ',	// 42:SHIFT(izq), 54:SHIFT(der), 55:*, 56:ALT/ALTGR, 57:SPACEBAR. 53 tb es '/'
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7',                          		// 58:BLOQ MAYUS, 59-68:F1-F10, 69:BLOQ NUM, 70:BLOQ DESPL, 71:INICIO y 7 (tecl num)
	'↑', '9', '-', '←', '5', '→', '+', '1', '↓', '3', 0, 0, 0, 0,				// 72:↑ y 8(tecl num), 73:REPAG y 9 (tecl num), 74:- (tecl num), 75:← y 4 (tecl num), 76:5 (tecl num), 77:→ y 6 (tecl num), 78:+ (tecl num), 79:FIN y 1 (tecl num), 80:↓ y 2 (tecl num), 81:AVPAG y 3 (tecl num), 82:INS, 83:SUPR, 84:?, 85:?
	0, 0, 0, 0, 0, 0, 0, 0														// 86:?, 87:F11, 88:F12, 89:?, 90:?, 91:TECLA WINDOWS, 92:?, 93:CLICK IZQUIERDO
};

extern uint8_t pollKeyRaw();

static int keyboardBufferIsFull(){
    if (getCurrentDisplay())
        return nextToStore_1 == BUFFER_SIZE;
    else
        return nextToStore_0 == BUFFER_SIZE;
}

static int keyboardbufferIsEmpty(){
    if (getCurrentDisplay())
        return currentToRead_1 == nextToStore_1;
    else
        return currentToRead_0 == nextToStore_0;
}

// Stores a key on the keyboard buffer
static void typeKey(int key){
    if (getCurrentDisplay()){
        if(keyboardBufferIsFull())
            nextToStore_1 = 0;
        // if(!keyboardBufferIsFull())
        keyboard_buffer_1[nextToStore_1++] = key;
    } else {
        if(keyboardBufferIsFull())
            nextToStore_0 = 0;
        // if(!keyboardBufferIsFull())
            keyboard_buffer_0[nextToStore_0++] = key;
    }
    
}

static void deleteLast (){
    if (getCurrentDisplay()){
        if(nextToStore_1 != currentToRead_1){
            if(nextToStore_1 == 0)
                nextToStore_1 = BUFFER_SIZE;
            nextToStore_1--;
        }
    } else {
        if(nextToStore_0 != currentToRead_0){
            if(nextToStore_0 == 0)
                nextToStore_0 = BUFFER_SIZE;
            nextToStore_0--;
        }
    }
    
}

// 0-31 and 127 are reserved ASCII control characters
static int isControlKey(unsigned char c){
	return c < 32 || c == 127;
}

static int isPrintableKey(unsigned char c){
    return c != '\t';
}

static void applyControlKey(unsigned char key){
    switch(key){
        case '\b':
            deleteLast();
            break;
        case '\n':
            if (getCurrentDisplay()){
                setInputBuffer(keyboard_buffer_1, currentToRead_1, nextToStore_1);
                currentToRead_1 = nextToStore_1;
            }else{
                setInputBuffer(keyboard_buffer_0, currentToRead_0, nextToStore_0);
                currentToRead_0 = nextToStore_0;
            }
            break;
        case '\t':
            swapDisplay();
            break;
        default:
            break;
    }
}

// Testing purposes. Delete when making final code
unsigned char getLastPressedKey(){
    return lastKey;
}

// Reads the input
// If there's a key, stores it on buffer/applies action on buffer and returns it
// If there's not a key, returns -1
static int readKey(){
    int key = -1;
    uint8_t keyCode = pollKeyRaw();
    // If it´s a MAKE, a key was pressed
    if(keyCode < 94){
        key = keyTable[keyCode];
        if(isControlKey(key))
            applyControlKey(key);
        else
            typeKey(key);
    }
    return key;
}

void keyboardIntHandler(){
    int key = readKey();
    if(key != -1 && isPrintableKey(key) && !keyboardBufferIsFull()){
        printChar(key);
        // printInt(nextToStore_0, 10);
        lastKey = key;      // Testing purposes. Delete when making final code
    }
}