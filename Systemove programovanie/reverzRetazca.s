# reverz.s
.global reverse

.text

reverse:
    # Prolog funkcie
    push    %rbp
    mov     %rsp, %rbp
    push    %r12
    push    %rbx

    # Ulozime si povodny pointer na retazec do %r12
    mov     %rdi, %r12        
    
    # Vypocitame dlzku retazca v argumente
    call    strlen             # zavolame strlen
    mov     %rax, %rcx         # ulozime dlzku do %rcx

    # Osetrime pripad, ked je retazec prazdny alebo len 1 znak
    cmp     $1, %rcx           # Ak je dlzka <= 1, taktiez vratime pointer
    jbe     koniec

    # Vypocitame si pointer na posledny znak retazca, t.j retazec[N-1] (pointer na zaciatok retazca + N-1)
    # (V ktorom registri je aktualne pointer na prvy znak retazca?)
    mov     %r12, %rsi         # %rsi = pointer na zaciatok retazca (lavy)
    lea     -1(%r12, %rcx), %rdx   # %rdx = pointer na koniec retazca (pravy)

cyklus:
    # Ked sa nam uz stretli indexy (t.j. ukazuju na ten isty prvok retazca alebo sa prekrizili), goto koniec
    cmp     %rsi, %rdx
    jb     koniec

    # Vymenime znaky na danych indexoch retazca.
    mov     (%rsi), %al
    mov     (%rdx), %bl
    mov     %bl, (%rsi)
    mov     %al, (%rdx)

    # Aktualizujeme pointre tak, aby jeden z nich ukazoval na nasledujuci znak a druhy na predosly znak retazca
    inc     %rsi
    dec     %rdx
    jmp     cyklus    

koniec:    
    # Vratime pointer na vysledok
    mov     %r12, %rax         # vratime povodny pointer na retazec
    pop     %rbx
    pop     %r12
    pop     %rbp
    ret

