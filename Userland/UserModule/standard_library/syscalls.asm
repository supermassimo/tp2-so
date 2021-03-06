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
GLOBAL memFree
GLOBAL getMemInfo
GLOBAL _createProcess
GLOBAL _nice
GLOBAL _block
; GLOBAL _exit
GLOBAL _kill
GLOBAL _printAllProcesses
GLOBAL _getpid
GLOBAL _skip
GLOBAL semOpen
GLOBAL semDestroy
GLOBAL semWait
GLOBAL semPost
GLOBAL semUpdateValue
GLOBAL printAllSemaphores
GLOBAL createPipe
GLOBAL closePipe
GLOBAL writePipe
GLOBAL readPipe
GLOBAL printPipes


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

memFree:
    mov rax, 13
    int 80h

    ret

getMemInfo:
    mov rax, 14
    int 80h

    ret

_createProcess:
    mov rax, 15
    int 80h

    ret

_nice:
    mov rax, 16
    int 80h

    ret

_block:
    mov rax, 17
    int 80h

    ret

_printAllProcesses:
    mov rax, 18
    int 80h

    ret

_skip:
    mov rax, 19
    int 80h

    ret

_getpid:
    mov rax, 39
    int 80h

    ret

; exit:
;    mov rax, 60
;    int 80h
;
;    ret

_kill:
    mov rax, 62
    int 80h

    ret

semOpen:
    mov rax, 64
    int 80h

    ret

semDestroy:
    mov rax, 65
    int 80h

    ret

semWait:
    mov rax, 66
    int 80h

    ret

semPost:
    mov rax, 67
    int 80h

    ret

semUpdateValue:
    mov rax, 68
    int 80h

    ret

printAllSemaphores:
    mov rax, 69
    int 80h

    ret

createPipe:
    mov rax, 70
    int 80h

    ret

closePipe:
    mov rax, 71
    int 80h

    ret

writePipe:
    mov rax, 72
    int 80h

    ret

readPipe:
    mov rax, 73
    int 80h

    ret

printPipes:
    mov rax, 74
    int 80h

    ret

invalidOpcodeThrower:
    ud2
    ret