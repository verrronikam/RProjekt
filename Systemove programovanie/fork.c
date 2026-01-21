#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int call_send_arg(int argc, char **argv) {
    // Validacia argumentov
    if (argc != 2) {
        fprintf(stderr, "Nespravne argumenty (cislo od 1 do 20)\n");
        fprintf(stderr, "./client <number>\n");
        return 1;
    }

    // Konverzia portu a validácia
    int N = atoi(argv[1]);
    if (N < 1 || N > 20) {
        fprintf(stderr, "Nespravne argumenty (cislo od 1 do 20)\n");
        fprintf(stderr, "./client <number>\n");
        return 1;
    }

    // Vypis 0 zo zakladného procesu
    printf("0\n");

    // Rekurzivne vytváranie procesov
    if (N >= 1) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
            return 1;
        }

        if (child_pid == 0) {
            // Detiakoy proces: zahadza rekurziu
            int current = 1;
            while (1) {
                printf("%d\n", current);

                if (current == N) {
                    return 0; // Ukoncenie pri dosiahnuti N
                }

                pid_t next_child = fork();
                if (next_child == -1) {
                    perror("fork");
                    return 1;
                }

                if (next_child > 0) {
                    // Rodic proces caka a ukonci sa
                    wait(NULL);
                    return 0;
                }

                current++;
            }
        } else {
            // Rodic proces caka na ukoncenie potomka
            wait(NULL);
        }
    }

    return 0;
}

int main(int argc, char **argv) {
    return call_send_arg(argc, argv);
}