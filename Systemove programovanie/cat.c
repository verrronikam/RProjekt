#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <string.h>
#include <fcntl.h>

#define BUFFSIZE 4096

int main(int argc, char **argv){
    int fdin, fdout;
    int rr, written;
    char buff[BUFFSIZE];
    
    if(argc > 3){
        printf("missing arguments\n");
        return 0;
        
    }
    
    if(argc >=2){
    // Ak meno suboru 1 je "-"
        if(strcmp(argv[1], "-") == 0){
            fdin = STDIN_FILENO;
        } else{
            fdin = open(argv[1], O_RDONLY);

            if (fdin == -1){
            printf("Nepodarilo sa otvorit vstupny subor '%s'.\n", argv[1]);
            return 0;
            }
        }
    } else {
        fdin = STDIN_FILENO;
    }
        
    // Ak meno suboru 2 je "-"
    if(argc == 3){
        if(strcmp(argv[2], "-") == 0){
            fdout = STDIN_FILENO;
        } else{
            fdout = open(argv[2], O_WRONLY | O_CREAT |O_TRUNC, 0664);
        
            if (fdout == -1){
            printf("Nepodarilo sa otvorit vystupny subor '%s'.\n", argv[2]);
            if(fdin!=STDIN_FILENO) close(fdin);
            return 0;
            }
        }
    
    } else {
        fdout = STDOUT_FILENO;
    }

    // Kopirujeme data
    while ((rr = read(fdin, buff, BUFFSIZE)) > 0) {
        char *ptr = buff;
        while (rr > 0) {
            written = write(fdout, ptr, rr);
            if (written == -1) {
                perror("write");
                break;
            }
            rr -= written;
            ptr += written;
        }
    }

    if (rr == -1) {
        perror("read");
    }

    if (fdin != STDIN_FILENO) close(fdin);
    if (fdout != STDOUT_FILENO) close(fdout);

    return 0;
}