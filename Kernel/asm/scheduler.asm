GLOBAL createPCB
GLOBAL scheduleNext

%macro pushState 0
	push rsp
	push 0x0	; r15
	push 0x1	; r14
	push 0x2	; r13
	push 0x3	; r12
	push 0x4	; r11
	push 0x5	; r10
	push 0x6	; r9
	push 0x7	; r8
	push rcx	; rsi
	push rdx	; rdi
	push 0xA	; rbp
	push 0xB	; rdx
	push 0xC	; rcx
	push 0xD	; rbx
	push 0xE	; rax
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
    push rsi          		; rsp
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