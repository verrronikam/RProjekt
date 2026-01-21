.global strlen_asm
.text

strlen_asm:
    # Prolog
    push %rbp
    mov %rsp, %rbp

    # RDI obsahuje pointer na retazec (char *s)

    mov %rdi, %rsi      # RSI ukazuje na zaciatok retazca
    mov $-1, %rcx       # RCX = maximalny pocet porovnani (0xFFFFFFFFFFFFFFFF)
    xor %al, %al        # Hladame NULL byte, preto AL = 0
    cld                 # Zabezpeci inkrementaciu pri SCASB (Clear Direction Flag)

    repne scasb         # HLADAJ NULL BYTE – porovnaj AL s bajtom na [RDI++], zmensuj RCX, pokial sa nerovna

    # Po ukonceni: RDI ukazuje na NULL bajt, RSI na zaciatok retazca
    # Dlzka = RDI - RSI - 1
    mov %rdi, %rax
    sub %rsi, %rax
    dec %rax            # Odstranime NULL bajt zo zapocitania

    # Epilog
    pop %rbp
    ret

