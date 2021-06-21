
GLOBAL getQuadratic

%macro ftest 0
    ftst
    fstsw ax            
    sahf
%endmacro

section .text

;de=(b*b)-4*(a*c);
;d=sqrt(de);
;root1=(-b+d) /(2.0*a);
;root2=(-b-d) /(2.0*a);

;xmm0 = a
;xmm1 = b
;xmm2 = c
;rcx = output*
getQuadratic:
    movq [a], xmm0
    movq [b], xmm1
    movq [c], xmm2

    fld qword [a]           ; stores a in the stack
    fscale                  ; multiply a by 2 then store in memory,
    fstp qword [two_a]      ; will be useful later

    fldz                
    fsub qword [b]          ; substract b from 0 then store in memory,               
    fstp qword [minus_b]    ; will be useful later

    fld qword [b]           ; push b to the stack twice
    fld qword [b]
    ;[b][b]

    fmulp                   ; multiply b by b, pops them and stores in the stack
    ;[b*b]

    fld qword [a]           ; push a, c and -4 to the stack
    fld qword [c]
    fild word [minus_four]
    ;[b*b][a][c][-4]

    fmulp                   ; multiply the last 3 values in the stack a, c and -4
    fmulp
    ;[b*b][-4*c*a]

    faddp                   ; add the last two values in the stack
    ;[(b*b)+(-4*c*a)]

    ftest                   ; tests the value in st0 ((b*b)+(-4*c*a))
    jl rootsAreImaginary    ; if its negative, the roots are imaginary
    je singleRoot           ; if its zero then its a single root

    fsqrt                   ; square roots st0 (sqrt(b*b)+(-4*c*a))
    ;[d]

    fst qword [d]           ; store d in memory, but dont pop

    fadd qword [minus_b]    ; add minus b to d
    ;[-b+d]

    fdiv qword [two_a]
    ;[root1]

    fstp qword [rcx]        ;set root 1 as the first value of the array 

    fld qword [minus_b]     ; push -b and substract d
    fsub qword [d]
    ;[-b-d]

    fdiv qword [two_a]
    ;[root2]

    fldpi

    fstp qword [rcx+4]      ;set root 2 as the second value of the array 

    mov rax, 2              ;set rax to 2, we found 2 roots

end:
    ret

rootsAreImaginary:
    mov rax, 0x0            ; no roots so we return 0
    jmp end

singleRoot:
    ;[(b*b)+(-4*c*a)]
    fcomp                   ;remove value from stack
    ;

    fld qword [minus_b]
    ;[-b]

    fdiv qword [two_a]      ; divide (-b) by (a*2)
    ;[root] 

    fstp qword [rcx]        ;set the root as the first value of the array

    mov rax, 1              ;set rax to 1, we found 1 root

    jmp end

section .data
    minus_four dw -4

section .bss
    a: resq 1
    b: resq 1
    c: resq 1
    two_a: resq 1
    minus_b: resq 1
    d: resq 1