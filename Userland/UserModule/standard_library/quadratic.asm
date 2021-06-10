
GLOBAL getQuadratic

section .text

;de=(b*b)-4*(a*c);
;d=sqrt(de);
;root1=(-b+d) /(2.0*a);
;root2=(-b-d) /(2.0*a);

;MOVSS [to here], [from here] move float
;DIVSS [div this], [by this] divide float
;MULSS [mult this], [by this] multiply float
;SQRTPS [destination], [sqrt of this] square root float
;ADDSS [this], [plus this] add floats
;SUBSS [this], [minus this] substract floats

;xmm0 = a
;xmm1 = b
;xmm2 = c
;rdi = output*
getQuadratic:
    ;[a][b][c][][][][][]

    movss xmm3, xmm1    ;move b to xmm3
    mulss xmm3, xmm1    ;multiply b by b (stored in xmm3)

    ;[a][b][c][b*b][][][][]
    
    movss xmm4, xmm0    ;move a to xmm4
    mulss xmm4, xmm2    ;multiply a by c (stored in xmm4)
    mulss xmm4, 4       ;multiply (a*c) by 4 (stored in xmm4)

    ;[a][b][c][b*b][a*c*4][][][]

    movss xmm5, xmm0    ;move a to xmm5
    mulss xmm5, 2       ;multiply a by 2 (stored in xmm5)

    ;[a][b][c][b*b][a*c*4][a*2][][]

    subss xmm3, xmm4    ;substract (b*b) minus (4*a*c) (stored in xmm3)

    ;[a][b][c][(b*b)-(a*c*4)][a*c*4][a*2][][]

    ucomiss xmm3, 0x0   ;if ((b*b)-(4*a*c)) is negative, roots are imaginary
    jb rootsAreImaginary
    je singleRoot

    sqrtps xmm6, xmm3   ;sqrt of ((b*b)-(4*a*c)) (stored in xmm4)

    ;[a][b][c][(b*b)-(a*c*4)][a*c*4][a*2][d][]

    movss xmm3, 0x0     
    subss xmm3, xmm1    ;set xmm3 to -b
    movss xmm4, xmm3    ;set xmm4 to -b

    ;[a][b][c][-b][-b][a*2][d][]

    addss xmm3, xmm4    ;add sqrt((b*b)-(a*c*4)) to -b
    subss xmm4, xmm4    ;substract sqrt((b*b)-(a*c*4)) from -b

    ;[a][b][c][-b+d][-b-d][a*2][d][]

    divss xmm3, xmm5    ;divide (-b+d) by (a*2)
    divss xmm4, xmm5    ;divide (-b-d) by (a*2)

    ;[a][b][c][root 1][root 2][a*2][d][]

    movss [rdi], xmm3   ;set root 1 as the first value of the array in rdi
    movss [rdi+4], xmm4 ;set root 2 as the second value of the array in rdi

    mov rax, 2          ;set rax to 2, we found 2 roots

end:
    ret

rootsAreImaginary:
    mov rax, 0x0
    jmp end

singleRoot:
    ;[a][b][c][(b*b)-(a*c*4)][a*c*4][a*2][][]

    movss xmm3, 0x0
    subss xmm3, xmm1    ;set xmm3 to -b

    ;[a][b][c][-b][a*c*4][a*2][][]

    divss xmm3, xmm5    ;divide (-b) by (a*2)

    ;[a][b][c][root][a*c*4][a*2][][]

    movss [rdi], xmm3   ;set the root as the first value of the array in rdi

    mov rax, 1          ;set rax to 1, we found 1 root

    jmp end