.section .rodata
fmt_rodic_arg: .string "Rodic: ciselny argument ma hodnotu %d\n"
rodic_spustam: .string "Rodic: spustam novy proces\n"
rodic_potomok_pid: .string "Rodic: potomok spusteny s PID %d\n"
potomok_pid: .string "Potomok: novy proces je spusteny s PID %d\n"
potomok_konci: .string "Potomok: proces konci\n"
rodic_koniec: .string "Rodic: potomok skoncil s navratovym kodom %d\n"
fork_error_msg: .string "Rodic: Fork skoncil s chybou: "

.section .data
timespec:
    .quad 3      # tv_sec = 3 seconds
    .quad 0      # tv_nsec = 0

.section .bss
    .lcomm status, 4

.section .text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp

    # Ulož argc a argv
    movq %rdi, %rbx         # argc
    movq %rsi, %r12         # argv

    # Parsuj argument
    cmpl $2, %ebx
    jl use_default
    movq 8(%r12), %rdi      # argv[1]
    call atoi
    movl %eax, %r13d        # exit code do r13d
    jmp after_parse

use_default:
    movl $172, %r13d

after_parse:
    # Vypíš argument
    movq $fmt_rodic_arg, %rdi
    movl %r13d, %esi
    xorl %eax, %eax
    call printf

    # Vypíš spúšťanie procesu
    movq $rodic_spustam, %rdi
    xorl %eax, %eax
    call printf

    # Fork
    movq $57, %rax          # syscall pre fork
    syscall

    cmpq $0, %rax
    jl fork_error           # chyba
    je child                # potomok

    # Rodičovský proces
    movq %rax, %r14         # PID potomka

    # Vypíš PID potomka
    movq $rodic_potomok_pid, %rdi
    movq %r14, %rsi
    xorl %eax, %eax
    call printf

    # Čakaj na potomka a zisti návratový kód
    movq $61, %rax          # wait4 syscall
    movl %r14d, %edi        # PID potomka
    leaq status(%rip), %rsi # adresa statusu
    xorl %edx, %edx         # žiadne voľby
    xorl %r10d, %r10d       # rusage = NULL
    syscall

    # Extrahuj návratový kód
    movl status(%rip), %eax
    shrl $8, %eax           # posun o 8 bitov pre exit code
    andl $0xFF, %eax        # maskuj na 8 bitov

    # Vypíš návratový kód
    movq $rodic_koniec, %rdi
    movl %eax, %esi
    xorl %eax, %eax
    call printf

    jmp exit_parent

child:
    # Získaj PID potomka
    movq $39, %rax          # getpid syscall
    syscall
    movq %rax, %rsi

    # Vypíš PID potomka
    movq $potomok_pid, %rdi
    xorl %eax, %eax
    call printf

    # Vypíš koniec potomka
    movq $potomok_konci, %rdi
    xorl %eax, %eax
    call printf

    # Čakaj 3 sekundy
    movq $35, %rax          # nanosleep syscall
    leaq timespec(%rip), %rdi
    xorq %rsi, %rsi         # neukladaj zvyšný čas
    syscall

    # Ukonči s návratovým kódom
    movq $60, %rax          # exit syscall
    movl %r13d, %edi
    syscall

fork_error:
    # Spracuj chybu fork
    negq %rax               # absolútna hodnota chyby
    movq %rax, %rdi
    call __errno_location   # získaj ukazovateľ na errno
    movl %edi, (%rax)       # ulož chybu do errno
    movq $fork_error_msg, %rdi
    call perror             # vypíš chybovú správu

exit_parent:
    # Ukonči rodičovský proces
    xorl %eax, %eax
    leave
    ret