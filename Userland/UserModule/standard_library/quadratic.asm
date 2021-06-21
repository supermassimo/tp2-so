
GLOBAL getQuadratic

section .text

;de=(b*b)-4*(a*c);
;d=sqrt(de);
;root1=(-b+d) /(2.0*a);
;root2=(-b-d) /(2.0*a);

;MOVQ [to here], [from here] move float
;DIVSS [div this], [by this] divide float
;MULQ [mult this], [by this] multiply float
;SQRTSS [destination], [sqrt of this] square root float
;ADDSS [this], [plus this] add floats
;SUBSS [this], [minus this] substract floats
;CVTSI2SS [float reg], [int reg] moves a value from an integer register to a float one
;XORPS [float reg 1], [float reg 2] XOR instruction for float registers

;xmm0 = a
;xmm1 = b
;xmm2 = c
;rcx = output*
getQuadratic:
    ;[a][b][c][][][][][]

    movq xmm3, xmm1    ;move b to xmm3
    mulq xmm3, xmm1    ;multiply b by b (stored in xmm3)

    ;[a][b][c][b*b][][][][]
    
    movq xmm4, xmm0    ;move a to xmm4
    mulq xmm4, xmm2    ;multiply a by c (stored in xmm4)
    mov rdx, 4
    cvtsi2ss xmm6, rdx
    mulq xmm4, xmm6       ;multiply (a*c) by 4 (stored in xmm4)

    ;[a][b][c][b*b][a*c*4][][4][]

    movq xmm5, xmm0    ;move a to xmm5
    mov rdx, 2
    cvtsi2ss xmm6, rdx
    mulq xmm5, xmm6       ;multiply a by 2 (stored in xmm5)

    ;[a][b][c][b*b][a*c*4][a*2][2][]

    subss xmm3, xmm4    ;substract (b*b) minus (4*a*c) (stored in xmm3)

    ;[a][b][c][(b*b)-(a*c*4)][a*c*4][a*2][2][]

    mov rdx, 0          ;set rdx to 0 then transform it into a float to compare with
    cvtsi2ss xmm6, rdx
    ucomiss xmm3, xmm6  ;if ((b*b)-(4*a*c)) is negative, roots are imaginary
    jl rootsAreImaginary
    ucomiss xmm3, xmm6  ;if ((b*b)-(4*a*c)) is negative, roots are imaginary
    je singleRoot

    sqrtss xmm6, xmm3   ;sqrt of ((b*b)-(4*a*c)) (stored in xmm4)

    ;[a][b][c][(b*b)-(a*c*4)][a*c*4][a*2][d][]

    xorps xmm3, xmm3    ;set xmm3 to 0
    subss xmm3, xmm1    ;set xmm3 to -b
    movq xmm4, xmm3    ;set xmm4 to -b

    ;[a][b][c][-b][-b][a*2][d][]

    addss xmm3, xmm4    ;add sqrt((b*b)-(a*c*4)) to -b
    subss xmm4, xmm4    ;substract sqrt((b*b)-(a*c*4)) from -b

    ;[a][b][c][-b+d][-b-d][a*2][d][]

    divss xmm3, xmm5    ;divide (-b+d) by (a*2)
    divss xmm4, xmm5    ;divide (-b-d) by (a*2)

    ;[a][b][c][root 1][root 2][a*2][d][]

    movq [rcx], xmm3   ;set root 1 as the first value of the array in rdi
    movq [rcx+4], xmm4 ;set root 2 as the second value of the array in rdi

    mov rax, 2          ;set rax to 2, we found 2 roots

end:
    ret

rootsAreImaginary:
    mov rax, 0x0
    jmp end

singleRoot:
    ;[a][b][c][(b*b)-(a*c*4)][a*c*4][a*2][][]

    xorps xmm3, xmm3    ;set xmm3 to zero
    subss xmm3, xmm1    ;set xmm3 to -b

    ;[a][b][c][-b][a*c*4][a*2][][]

    divss xmm3, xmm5    ;divide (-b) by (a*2)

    ;[a][b][c][root][a*c*4][a*2][][]

    movq [rdi], xmm3   ;set the root as the first value of the array in rdi

    mov rax, 1          ;set rax to 1, we found 1 root

    jmp end

section .data
  minusFour dw 4

section .bss
a: resq 1
  b: resq 1
  c: resq 1