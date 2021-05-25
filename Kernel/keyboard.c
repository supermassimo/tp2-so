#include <keyboard.h>
#include <console.h>

#define BUFFER_SIZE 256

const int keyTable[] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', 168,			// 1:ESC
	'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '´', '+',		// 14:BACKSPACE
	'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 164, '{', '|',		// 29:CTRL
	15, '}', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0, '*', 0, ' ',	// 42:SHIFT(izq), 54:SHIFT(der), 55:*, 56:ALT/ALTGR, 57:SPACEBAR. 53 tb es '/'
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7',                          		// 58:BLOQ MAYUS, 59-68:F1-F10, 69:BLOQ NUM, 70:BLOQ DESPL, 71:INICIO y 7 (tecl num)
	'↑', '9', '-', '←', '5', '→', '+', '1', '↓', '3', 0, 0, 0, 0,				// 72:↑ y 8(tecl num), 73:REPAG y 9 (tecl num), 74:- (tecl num), 75:← y 4 (tecl num), 76:5 (tecl num), 77:→ y 6 (tecl num), 78:+ (tecl num), 79:FIN y 1 (tecl num), 80:↓ y 2 (tecl num), 81:AVPAG y 3 (tecl num), 82:INS, 83:SUPR, 84:?, 85:?
	0, 0, 0, 0, 0, 0, 0, 0														// 86:?, 87:F11, 88:F12, 89:?, 90:?, 91:TECLA WINDOWS, 92:?, 93:CLICK IZQUIERDO
};

extern uint8_t pollKeyRaw();

static int buffer[BUFFER_SIZE];
static unsigned int nextToStore = 0;
static unsigned int currentToRead = 0;

static int bufferIsFull(){
    if(nextToStore == BUFFER_SIZE)
        return 1;
    return 0;
}

// Stores a key on the buffer
static void storeKey(uint8_t keyCode){
    if(bufferIsFull())
        nextToStore = 0;
    buffer[nextToStore++] = keyTable[keyCode];
}

// Returns last stored key without removing it from buffer
int peekLastKey(){
    int last = nextToStore - 1;
    if(last == -1)
        last = BUFFER_SIZE - 1;
    return buffer[last];
}

int getBufferSize(){
    return BUFFER_SIZE;
}

// If there are keys on buffer, returns the first available. Else returns -1
int getKey(){
    if(currentToRead != nextToStore){
        if(currentToRead == BUFFER_SIZE)
            currentToRead = 0;
        return buffer[currentToRead++];
    }
    return -1;
}

// Fills array sent with buffer content on string format (ends with '\0') 
// and returns # of keys read from buffer
int getBufferContent(unsigned char* target){
    int i=0, aux;
    while((aux = getKey()) != -1){
        target[i++] = aux;
    }
    target[i] = 0;
    return i;
}

// Reads the input
// If there's a key, stores it on buffer and returns 1
// If there's not a key, returns 0
static int readKey(){
    uint8_t keyCode = pollKeyRaw();
    if(keyCode < 128){
        storeKey(keyCode);
        return 1;  
    } 
    return 0;    
}  

void keyboardIntHandler(){
    if(readKey())
        printChar(peekLastKey());
}