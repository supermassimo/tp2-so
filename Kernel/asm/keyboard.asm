GLOBAL pollKeyRaw

section .text

pollKeyRaw:
			push rbp
			mov rbp, rsp

			xor rax, rax
	_loop:	in al, 64h
			and al, 01h
			jz _loop
			in al, 60h
			leave
			ret