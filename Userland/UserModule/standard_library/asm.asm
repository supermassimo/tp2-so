GLOBAL readInput
GLOBAL write
GLOBAL writeRegistries
GLOBAL writeMemContent

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