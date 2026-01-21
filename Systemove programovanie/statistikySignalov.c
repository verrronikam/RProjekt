#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXSIGS 32

const char sig_names[][32] = {
    "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP",
    "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1",
    "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM",
    "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP",
    "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ",
    "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGIO", "SIGPWR",
    "SIGSYS", "UNKNOWN"
};

volatile int signal_counts[MAXSIGS] = {0};
volatile int unknown_signal_count = 0;

// Funkcia pre obsluhu signalov
void signal_handler(int sig) {
    if (sig >= 1 && sig <= MAXSIGS - 1) {
        signal_counts[sig]++;
        printf("Caught %s (signal number %d)\n", sig_names[sig - 1], sig);
    } else {
        unknown_signal_count++;
        printf("Caught unknown (signal number %d)\n", sig);
    }

    // Znovu nastavenie obsluhy signalu
    signal(sig, signal_handler);
}

// Funkcia pre obsluhu signalu SIGTERM
void sigterm_handler(int sig) {
    printf("Finished. Signal statistics follows:\n");
    for (int i = 1; i < MAXSIGS; i++) {
        printf("Signal %s\tcount: %3d\n", sig_names[i - 1], signal_counts[i]);
    }
    printf("Signal UNKNOWN\tcount: %3d\n", unknown_signal_count);
    exit(0);
}

int catchall() {
    // Nastavenie obsluhy signalov pre cisla 1 az 31
    for (int i = 1; i <= MAXSIGS - 1; i++) {
        if (signal(i, signal_handler) == SIG_ERR) {
            perror("signal() error: ");
            exit(1);
        }
    }

    // Nastavenie obsluhy signalu SIGTERM
    if (signal(SIGTERM, sigterm_handler) == SIG_ERR) {
        perror("signal() error: ");
        exit(1);
    }

    printf("Starting catcher\n");

    // Nekonecna slucka
    while (1) {
        sleep(1); // Pridane pre efektivne ckanie
    }

    return 0;
}

int main() {
    return catchall();
}