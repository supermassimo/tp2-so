GLOBAL readInput
GLOBAL write
GLOBAL writeRegistries
GLOBAL writeMemContent
GLOBAL writeDateTime
GLOBAL setIdle
GLOBAL clear
GLOBAL writeCpuFeatures
GLOBAL invalidOpcodeThrower
GLOBAL swapConsole

section .text

readInput:
    mov rbx, 0 ; pasaje de parametro
	int 69h

    ret

write:
    mov rbx, 1 ; pasaje de parametro
	int 69h

    ret

writeRegistries:
    mov rbx, 2 ; pasaje de parametro
	int 69h

    ret

writeMemContent:
    mov rbx, 3
    int 69h

    ret

writeDateTime:
    mov rbx, 4
    int 69h

    ret

setIdle:
    mov rbx, 5
    int 69h

    ret

clear:
    mov rbx, 6
    int 69h

    ret

writeCpuFeatures:
    mov rbx, 8
    int 69h

    ret

swapConsole:
    mov rbx, 9
    int 69h

    ret

invalidOpcodeThrower:
    ud2
    ret