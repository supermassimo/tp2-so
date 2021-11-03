GLOBAL lockAdd
GLOBAL lockSet

lockAdd:
    lock xadd [rdi], rsi
    ret

lockSet:
    lock xchg [rdi], rsi
    ret