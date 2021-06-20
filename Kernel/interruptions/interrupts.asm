
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL _sysCallHandler

EXTERN sysReadInput
EXTERN sysWrite
EXTERN sysWriteRegistries
EXTERN sysWriteMemContent
EXTERN sysWriteDateTime
EXTERN sysSetIdle
EXTERN sysClear
EXTERN sysGetActiveDisplay
EXTERN sysWriteCpuFeatures
EXTERN sysSwapActiveDisplay
EXTERN sysSleep

EXTERN irqDispatcher
EXTERN exceptionDispatcher

EXTERN sysCallDispatcher

EXTERN rebootKernel

EXTERN awaitForInstantInput

USER_MODULE_ADDRESS EQU 0x400000

SECTION .text


%macro pushState 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push rax
%endmacro

%macro popState 0
	pop rax
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	call _hlt
	call awaitForInstantInput
	;call rebootKernel
	popState
	add rsp, 8
	push USER_MODULE_ADDRESS
	iretq
%endmacro

_sysCallHandler:
		pushState
		cmp rax, 0
		je syscall_0
		cmp rax, 1
		je syscall_1
		cmp rax, 2
		je syscall_2
		cmp rax, 3
		je syscall_3
		cmp rax, 4
		je syscall_4
		cmp rax, 5
		je syscall_5
		cmp rax, 6
		je syscall_6
		cmp rax, 7
		je syscall_7
		cmp rax, 8
		je syscall_8
		cmp rax, 9
		je syscall_9
		cmp rax, 10
		je syscall_10
	syscall_0:
		call sysReadInput
		jmp endSysCallHandler
	syscall_1:
		call sysWrite
		jmp endSysCallHandler
	syscall_2:
		call sysWriteRegistries
		jmp endSysCallHandler
	syscall_3:
		call sysWriteMemContent
		jmp endSysCallHandler
	syscall_4:
		call sysWriteMemContent
		jmp endSysCallHandler
	syscall_5:
		call sysSetIdle
		jmp endSysCallHandler
	syscall_6:
		call sysClear
		jmp endSysCallHandler
	syscall_7:
		call sysGetActiveDisplay
		jmp endSysCallHandler
	syscall_8:
		call sysWriteCpuFeatures
		jmp endSysCallHandler
	syscall_9:
		call sysSwapActiveDisplay
		jmp endSysCallHandler
	syscall_10:
		call sysSleep
		jmp endSysCallHandler
		
	endSysCallHandler:
		popState
		iretq

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Operation Code
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1
	regs resq 15