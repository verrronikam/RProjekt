.global strlen_asm
.global strnlen_asm
.text


# int strlen_asm(char *s);

strlen_asm:
    push %rbp
    mov %rsp, %rbp

    mov %rdi, %rsi      # RSI = zaciatok retazca
    xor %al, %al        # Hladame NULL bajt
    cld                 # Inkrementujme smer vyhladavania
    mov $-1, %rcx       # max hladanie

    repne scasb         # Porovnavajme bajty az kym nenajdes 0

    lea -1(%rdi), %rax  # RAX = adresa nulloveho bajtu
    sub %rsi, %rax      # length = end - start

    pop %rbp
    ret


# int strnlen_asm(char *s, int n);

strnlen_asm:
    push %rbp
    mov %rsp, %rbp

    test %rsi, %rsi         # Ak n == 0
    je .zero_length         # vrat 0

    mov %rdi, %r8           # povodny smernik na r8
    mov %rsi, %r9          # RCX = n
    mov %r9, %rcx          # RSI = zaciatok retazca (pre sub)
    xor %al, %al            # hladame NULL bajt
    cld                     # Clear direction flag (pre inkrementaciu)

    repne scasb             # Hladajme NULL bajt max 'n' bajtov

    jne .not_found          # ak nenajdeme, vratime n

    lea -1(%rdi), %rax      # RAX = adresa nuloveho terminatora
    sub %r8, %rax           # RAX = length (adresa nuly - start adresa)
    jmp .done

.not_found:
    mov %r9, %rax          # RAX = n

.done:
    pop %rbp 
    ret

.zero_length:
    xor %rax, %rax
    pop %rbp
    ret

