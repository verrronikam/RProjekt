.global euklid

.text

euklid:
    # Prológ
    push %rbp
    mov %rsp, %rbp

    # Ak b == 0, vrat a
    cmp $0, %rsi
    je .end

    # Vypocitaj a % b
    mov %rdi, %rax       # a do RAX
    xor %rdx, %rdx       # vynuluj RDX (div potrebuje RDX:RAX)
    div %rsi             # RAX = a / b, RDX = a % b

    # Zavolaj rekurzivne euklid(b, a % b)
    mov %rsi, %rdi       # nove a = b
    mov %rdx, %rsi       # nove b = a % b
    call euklid

    jmp .ret

.end:
    mov %rdi, %rax       # vysledok = a

.ret:
    # Epilóg
    mov %rbp, %rsp
    pop %rbp
    ret
