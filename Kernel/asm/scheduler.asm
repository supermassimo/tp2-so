GLOBAL createPCB
GLOBAL scheduleNext

%macro pushState 0
	push rsp
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push r8		; rsi: argv
	push rcx	; rdi: argc
	push rbp
	push rdx
	push rcx
	push rbx
	push rax
%endmacro

%macro popState 0
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rbp
	pop rdi
	pop rsi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
	pop rsp
%endmacro

; createPCB(void* entryPoint, void* pcbAddr, void* stackAddr)
createPCB:
    push rbp
    mov rbp, rsp

    mov rsp, rsi
    push 0x0                ; ss
    push rdx          		; rsp
    push 0x202              ; rflags
    push 0x8                ; cs
    push rdi                ; rip
    pushState

    mov rax, rsp
    leave
    ret

scheduleNext:
	int 20h
	ret