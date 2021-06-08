#include <console.h>
//Dual Integrated Logical Display Output

static int currentDisplay = 0;
static int waitingToChange = 0;
static int isIdle = 1;

void setIdle (int value){
    isIdle = !(!value); //double negative to force value to be 0 or 1
    if (!isIdle && waitingToChange){
        swapDisplay();
        waitingToChange = 0;
    }
}

void swapDisplay (){
    if (isIdle){
        currentDisplay = !currentDisplay;
        changeConsoleSide(currentDisplay);
    } else {
        waitingToChange = 1;
    }
}

int getCurrentDisplay () {
    return currentDisplay;
}
