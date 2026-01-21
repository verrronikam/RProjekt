#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Nespravne argumenty (cislo od 1 do 20)\n");
        return 1;
    }

    char *endptr;
    long N = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || N < 1 || N > 20) {
        printf("Nespravne argumenty (cislo od 1 do 20)\n");
        return 1;
    }

    for (int i = 0; i <= N; i++) {
        printf("%d\n", i);
        fflush(stdout);

        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Fork skoncil s chybou: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        } 
    }

    return 0;
}
