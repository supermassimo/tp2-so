GLOBAL readInput
GLOBAL write
GLOBAL writeRegistries

%macro syscallHandler 1
	mov rax, %1 ; pasaje de parametro
	int 0x69

    ret
%endmacro

section .text

readInput:
    syscallHandler 0

write:
    syscallHandler 1

writeRegistries
    syscallHandler 2
