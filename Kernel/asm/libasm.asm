GLOBAL cpuVendor
GLOBAL getCpuFeaturesEDXRaw
GLOBAL getCpuFeaturesECXRaw
GLOBAL getCpuExtendedFeaturesEBXRaw
GLOBAL getCpuExtendedFeaturesECXRaw

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