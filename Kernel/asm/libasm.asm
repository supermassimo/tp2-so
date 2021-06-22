GLOBAL cpuVendor
GLOBAL getCpuFeaturesEDXRaw
GLOBAL getCpuFeaturesECXRaw
GLOBAL getCpuExtendedFeaturesEBXRaw
GLOBAL getCpuExtendedFeaturesECXRaw

GLOBAL getCpuIdSupport

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getCpuFeaturesEDXRaw:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 1
	cpuid

	mov eax, edx
	pop rbx
	
	leave
	ret

	
getCpuFeaturesECXRaw:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 1
	cpuid

	mov eax, ecx
	pop rbx
	
	leave
	ret

getCpuExtendedFeaturesEBXRaw:
	push rbp
	mov rbp, rsp

	push rbx
	mov rax, 7
	mov rcx, 0
	cpuid

	mov eax, ebx
	pop rbx

	leave
	ret

getCpuExtendedFeaturesECXRaw:
	push rbp
	mov rbp, rsp

	push rbx
	mov rax, 7
	mov rcx, 0
	cpuid

	mov eax, ecx
	pop rbx

	leave
	ret

; If software can toggle the value of bit 21 on EFLAGS register, then the CPUID instruction is executable
; (Intel's Processor Identification and the CPUID Instruction, May 2012)
getCpuIdSupport:
	push rbp
	mov rbp, rsp
	pushfq				; Pushes EFLAGS register into stack
	pop rax
	mov rdx, rax		; Save current state of flags on rdx
	xor rax, 200000h	; Toggle bit 21 -> 0 xor 1 = 1
	push rax			;				   1 xor 1 = 0
	popfq				; Put the changed flags register (bit 21 toggled) back in place
	pushfq
	pop rax				; Get current flags register
	cmp rdx, rax		; Check if CPU changed bit 21
	je _noCpuId
	mov rax, 1
	leave
	ret
_noCpuId:
	mov rax, 0
	leave
	ret
