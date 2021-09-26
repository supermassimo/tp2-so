GLOBAL readInput
GLOBAL write
GLOBAL getRegistries
GLOBAL getMemContent
GLOBAL getDateTime
GLOBAL setIdle
GLOBAL clear
GLOBAL getCpuFeatures
GLOBAL invalidOpcodeThrower
GLOBAL swapConsole
GLOBAL sleep
GLOBAL delKey
GLOBAL memAlloc

section .text

readInput:
    mov rax, 0 ; pasaje de parametro
	int 80h

    ret

write:
    mov rax, 1 ; pasaje de parametro
	int 80h

    ret

getRegistries:
    mov rax, 2 ; pasaje de parametro
	int 80h

    ret

getMemContent:
    mov rax, 3
    int 80h

    ret

getDateTime:
    mov rax, 4
    int 80h

    ret

setIdle:
    mov rax, 5
    int 80h

    ret

clear:
    mov rax, 6
    int 80h

    ret

getCpuFeatures:
    mov rax, 8
    int 80h

    ret

swapConsole:
    mov rax, 9
    int 80h

    ret

sleep:
    mov rax, 10
    int 80h

    ret

delKey:
    mov rax, 11
    int 80h

    ret

memAlloc:
    mov rax, 12
    int 80h

    ret

invalidOpcodeThrower:
    ud2
    ret