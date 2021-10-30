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
GLOBAL createProcess
GLOBAL nice
GLOBAL block
GLOBAL exit
GLOBAL kill
GLOBAL printAllProcesses
GLOBAL getpid
GLOBAL skip
GLOBAL semInit
GLOBAL semDestroy
GLOBAL semWait
GLOBAL semPost
GLOBAL semUpdateValue

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

createProcess:
    mov rax, 15
    int 80h

    ret

nice:
    mov rax, 16
    int 80h

    ret

block:
    mov rax, 17
    int 80h

    ret

printAllProcesses:
    mov rax, 18
    int 80h

    ret

skip:
    mov rax, 19
    int 80h

    ret

getpid:
    mov rax, 39
    int 80h

    ret

exit:
    mov rax, 60
    int 80h

    ret

kill:
    mov rax, 62
    int 80h

    ret

semInit:
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

invalidOpcodeThrower:
    ud2
    ret