#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

// Funkcia my_kill posia signal procesu s danym PID
int my_kill(pid_t pid, int sig) {
    // Volame syst movu kill
    if (kill(pid, sig) == -1) {
        // Ak nastane chyba, vypiseme popis chyby vo formate "kill(pid, %d): %s"
        fprintf("kill(%d, %d): %s\n", pid, sig, strerror(errno));
        return 1;
    }
    return 0;
}

// Funkcia call_kill_arg spracuje argumenty prikazoveho riadku a vola my_kill
int call_kill_arg(int argc, char **argv) {
    // Kontrola poctu argumentov
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <signal> <pid>\n", argv[0]);
        return 1;
    }

    // Konverzia argumentov na cisla
    int sig = atoi(argv[1]);
    pid_t pid = (pid_t)atoi(argv[2]);

    // Kontrola platnosti cisiel signalu a PID
    if (sig < 0 || pid <= 0) {
        fprintf(stderr, "Invalid signal or pid\n");
        return 1;
    }

    // Volanie funkcie my_kill
    return my_kill(pid, sig);
}