.globl fibonacci
.text

fibonacci:
    # Prolog funkcie
    push %rbp
    mov %rsp, %rbp

    # if (n <= 1) return n;
    cmp $1, %rdi       # porovnajme n s 1
    jg recurse         # ak n > 1, skocime na rekurziu

    mov %rdi, %rax     # vratime n ako vysledok
    pop %rbp
    ret

recurse:
    # Zavolame fibonacci(n - 1)
    push %rdi          # ulozime n, lebo ho budeme este potrebovat
    dec %rdi           # n - 1
    call fibonacci     # volame fibonacci(n - 1)
    mov %rax, %rbx     # ulozime vysledok fibonacci(n - 1) do rbx

    pop %rdi           # obnova povodneho n
    push %rbx          # docasne ulozime fibonacci(n - 1)

    sub $2, %rdi       # n - 2
    call fibonacci     # volanie fibonacci(n - 2)

    pop %rbx           # nacitame fibonacci(n - 1)
    add %rbx, %rax     # vysledok = fibonacci(n - 1) + fibonacci(n - 2)

    # Epilog
    pop %rbp
    ret
