
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

GLOBAL captureRegistries

EXTERN setIdle

; Syscalls
EXTERN sysReadInput
EXTERN sysWrite
EXTERN sysGetMemContent
EXTERN sysGetDateTime
EXTERN sysSetIdle
EXTERN sysClear
EXTERN sysGetActiveDisplay
EXTERN sysGetCpuFeatures
EXTERN sysSwapActiveDisplay
EXTERN sysSleep
EXTERN sysDelKey
EXTERN sysMemAlloc
EXTERN sysMemFree
EXTERN sysGetMemInfo
EXTERN sysCreateProcess
EXTERN sysNice
EXTERN sysBlock
EXTERN sysExit
EXTERN sysKill
EXTERN sysPrintAllProcesses
EXTERN sysGetpid
EXTERN sysSkip
EXTERN sysSemInit
EXTERN sysSemDestroy
EXTERN sysSemWait
EXTERN sysSemPost
EXTERN sysSemSetValue

EXTERN irqDispatcher
EXTERN exceptionDispatcher

EXTERN sysCallDispatcher

EXTERN printRegistries
EXTERN rebootKernel

EXTERN awaitForInstantInput
EXTERN schedule
EXTERN scheduleOutsideRtc

USER_MODULE_ADDRESS EQU 0x400000

SECTION .text

; RIP is value in ret, so no need to push it in register stack
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
	push rsi
	push rdi
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

%macro irqHandlerMaster 1
	pushState
	mov [reg_stack_pointer], rsp

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

	mov rdi, rsp
	call printRegistries

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	add rsp, 8
	push USER_MODULE_ADDRESS
	mov rdi, 1
	call setIdle
	iretq
%endmacro

_sysCallHandler:
		pushState
		mov [reg_stack_pointer], rsp
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
		cmp rax, 11
		je syscall_11
		cmp rax, 12
		je syscall_12
		cmp rax, 13
		je syscall_13
		cmp rax, 14
		je syscall_14
		cmp rax, 15
		je syscall_15
		cmp rax, 16
		je syscall_16
		cmp rax, 17
		je syscall_17
		cmp rax, 18
		je syscall_18
		cmp rax, 19
		je syscall_19
		cmp rax, 39
		je syscall_39
		cmp rax, 60
		je syscall_60
		cmp rax, 62
		je syscall_62
		
		cmp rax, 64
		je syscall_64
		cmp rax, 65
		je syscall_65
		cmp rax, 66
		je syscall_66
		cmp rax, 67
		je syscall_67
		cmp rax, 68
		je syscall_68
	syscall_0:
		call sysReadInput
		jmp endSysCallHandler
	syscall_1:
		call sysWrite
		jmp endSysCallHandler
	; [RAX, RBX, RCX, RDX, RBP, RDI, RSI, R8, R9, R10, R11, R12, R13, R14, R15, RSP, RIP]
	syscall_2:
		mov rbx, regs_arr
		call getRegisters
		jmp endSysCallHandler
	syscall_3:
		call sysGetMemContent
		jmp endSysCallHandler
	syscall_4:
		call sysGetDateTime
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
		call sysGetCpuFeatures
		jmp endSysCallHandler
	syscall_9:
		call sysSwapActiveDisplay
		jmp endSysCallHandler
	syscall_10:
		call sysSleep
		jmp endSysCallHandler
	syscall_11:
		call sysDelKey
		jmp endSysCallHandler
	syscall_12:
		call sysMemAlloc
		jmp endSysCallHandler
	syscall_13:
		call sysMemFree
		jmp endSysCallHandler
	syscall_14:
		call sysGetMemInfo
		jmp endSysCallHandler
	syscall_15:
		call sysCreateProcess
		jmp endSysCallHandler
	syscall_16:
		call sysNice
		jmp endSysCallHandler
	syscall_17:
		call sysBlock
		jmp endSysCallHandler
	syscall_18:
		call sysPrintAllProcesses
		jmp endSysCallHandler
	syscall_19:
		call sysSkip
		jmp endSysCallHandler
	syscall_39:
		call sysGetpid
		jmp endSysCallHandler
	syscall_60:
		call sysExit
		jmp endSysCallHandler
	syscall_62:
		call sysKill
		jmp endSysCallHandler
	syscall_64:
		call sysSemInit
		jmp endSysCallHandler
	syscall_65:
		call sysSemDestroy
		jmp endSysCallHandler
	syscall_66:
		call sysSemWait
		jmp endSysCallHandler
	syscall_67:
		call sysSemPost
		jmp endSysCallHandler		
	syscall_68:
		call sysSemSetValue
		jmp endSysCallHandler	
	endSysCallHandler:
		mov rbx, [reg_stack_pointer]
		mov [rbx], rax
		popState
		iretq

captureRegistries:
	mov rbx, [reg_stack_pointer]
	mov rdi, regs_arr
	call getRegisters
	ret

; RBX -> Address of register stack
; RDI -> Address of target array
getRegisters:
		    push rbp
		    mov rbp, rsp
            push rcx
            mov rcx, 0
        getRegLoop:
            cmp rcx, regs_len
            je getRegEnd
			mov rax, [rbx]
			mov [rdi], rax
			add rcx, 1
			add rdi, 8
			add rbx, 8
			jmp getRegLoop
        getRegEnd:
            pop rcx
		    leave
		    ret

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
		pushState
		mov [reg_stack_pointer], rsp

		; Scheduler
		mov rdi, rsp
		call schedule
		mov rsp, rax

		call scheduleOutsideRtc
		cmp rax, 1
		je endInt00

		; Timer
		mov rdi, 0
		call irqDispatcher

	endInt00:
		mov al, 20h
		out 20h, al

		popState
		iretq
	
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

SECTION .data
	regs_len equ 17
SECTION .bss
	aux resq 1
	reg_stack_pointer resq 1
	regs_arr resq regs_len