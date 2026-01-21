#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void print_permissions(mode_t mode) {
    char permissions[] = "---------";
    
    if (mode & S_IRUSR) permissions[0] = 'r';
    if (mode & S_IWUSR) permissions[1] = 'w';
    if (mode & S_IXUSR) permissions[2] = 'x';
    
    if (mode & S_IRGRP) permissions[3] = 'r';
    if (mode & S_IWGRP) permissions[4] = 'w';
    if (mode & S_IXGRP) permissions[5] = 'x';
    
    if (mode & S_IROTH) permissions[6] = 'r';
    if (mode & S_IWOTH) permissions[7] = 'w';
    if (mode & S_IXOTH) permissions[8] = 'x';
    
    printf("%.9s ", permissions);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Arguments missing.\n");
        return 0;
    }
    
    for (int i = 1; i < argc; i++) {
        struct stat file_stat;
        
        if (stat(argv[i], &file_stat) == -1) {
            printf("'%s' error opening file\n", argv[i]);
            continue;
        }
        
        print_permissions(file_stat.st_mode);
        
        if (S_ISREG(file_stat.st_mode)) {
            printf("%s %ld\n", argv[i], (long)file_stat.st_size);
        } else {
            printf("%s not a regular file\n", argv[i]);
        }
    }
    
    return 0;
}