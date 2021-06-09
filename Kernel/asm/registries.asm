GLOBAL getRIP
GLOBAL getRAX
GLOBAL getRBX
GLOBAL getRCX
GLOBAL getRDX
GLOBAL getRBP
GLOBAL getRDI
GLOBAL getRSI
GLOBAL getR8
GLOBAL getR9
GLOBAL getR10
GLOBAL getR11
GLOBAL getR12
GLOBAL getR13
GLOBAL getR14
GLOBAL getR15

section .text

;RAX is already the return value.
;Must be called before the others as RAX will get overridden otherwise
getRAX:
	ret

getRBX:
	mov rax, rbx
	ret

getRCX:
	mov rax, rcx
	ret

getRDX:
	mov rax, rdx
	ret

getRBP:
	mov rax, rbp
	ret

getRDI:
	mov rax, rdi
	ret

getRSI:
	mov rax, rsi
	ret

getR8:
	mov rax, r8
	ret

getR9:
	mov rax, r9
	ret

getR10:
	mov rax, r10
	ret

getR11:
	mov rax, r11
	ret

getR12:
	mov rax, r12
	ret

getR13:
	mov rax, r13
	ret

getR14:
	mov rax, r14
	ret

getR15:
	mov rax, r15
	ret

getRIP:
	call _RIP
_RIP: pop rax
	ret