.global strncat_asm

.data

.bss

.text


strncat_asm:
	# argumenty: 1 - dest, 2 - src, 3 - n

	# Prolog funkcie
	push %rbp
	mov %rsp, %rbp

    mov %rdi, %r8       # ponodny dst smernik pre return

	# Zistenie konca dst
    xor %al, %al          # AL = 0 (null terminator)
    mov $-1, %rcx         # Max pocet pre scasb
    cld                   # Clear direction flag (forward)

	repnz scasb

    dec %rdi              # Nastavime RDI aby ukazoval na nulovy terminator

    # Ak n = 0
    test %edx, %edx
    jz fin

    # Nastavime loop counter s n
    mov %edx, %ecx

again:
	 cmpb $0, (%rsi)       # ak src je nula
    je fin
	movsb                  # kopirujeme bajt z src do dst
	loop again
fin:
	# navratova hodnota
    movb $0, (%rdi)
    # Vratime povodny dst smernik
    mov %r8, %rax

	# Epilog funkcie
	leave
	ret