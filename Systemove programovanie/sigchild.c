#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid_child;

// Funkcia pre obsluhu signalu SIGCHLD
void sigchld_handler(int sig) {
    int status;
    pid_t pid;

    // Zistenie stavu dcerskeho procesu
    pid = waitpid(pid_child, &status, WNOHANG);
    if (pid > 0) {
        printf("PARENT: caught SIGCHLD signal (number %d)\n", sig);
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf("PARENT: child finished with exit code: %d\n", exit_code);
        }
    }
    // Znovu nastavenie obsluhy signalu SIGCHLD
    signal(SIGCHLD, sigchld_handler);
}

int main(int argc, char **argv) {
    unsigned int p = 0, n = (argc == 2) ? atoi(argv[1]) : 50;

    // Nastavenie obsluhy signalu SIGCHLD
    if (signal(SIGCHLD, sigchld_handler) == SIG_ERR) {
        perror("PARENT: signal() error: ");
        exit(1);
    }

    fflush(stdout);
    pid_child = fork();
    if (pid_child == 0) {
        // Child process; do some work
        n *= 10000000;
        while (p++ < n) {
            if (!(p % (n / 10))) {
                printf("CHILD process: working %d%%\n", p / (n / 100));
            }
        }
        p /= 10000000;
        printf("CHILD process: finished, exit code set to: %d\n", p);
        exit(p);

    } else if (pid_child > 0) {
        // Parent process
        printf("PARENT: new child process created\n");
        printf("PARENT: start working\n");
        while (p++ < n * 1000000000) {
            if (((p + 1) % 10) > 11 + p) {
                printf("\n");
            }
        }
        printf("PARENT: finished\n");

    } else {
        perror("PARENT: fork() error: ");
        exit(1);
    }

    return 0;
}