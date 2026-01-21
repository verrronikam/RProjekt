#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int no_newline = 0;
    int start = 1;

    // Overenie ci prvy argument je "-n"
    if (argc >= 2 && strcmp(argv[1], "-n") == 0) {
        no_newline = 1;
        start = 2;
    }

    int first = 1;
    for (int i = start; i < argc; ++i) {
        // Vypisujeme argumenty oddelene medzerou
        if (!first) {
            putchar(' ');
        } else {
            first = 0;
        }
        fputs(argv[i], stdout);
    }

    // Ak je nastaveny prepinac -n, nevypisujeme novy riadok
    // Inak vypisujeme novy riadok
    if (!no_newline) {
        putchar('\n');
    }

    return 0;
}