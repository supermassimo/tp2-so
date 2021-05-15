GLOBAL setRtcReg
GLOBAL getRtcReg

section .text

getRtcReg:
	push rbp
	mov rbp, rsp

	mov eax, edi
	out 70h, al
	in al, 71h		 ; Get the desired register data (defined by parameter sent)

	leave
	ret

setRtcReg:
	push rbp
	mov rbp, rsp

	mov eax, edi
	out 70h, al
	mov eax, esi
	out 71h, al		 ; Set the desired register data (defined by parameter sent)

	leave
	ret
