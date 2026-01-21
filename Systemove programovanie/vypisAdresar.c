#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> // pre opendir(), readdir(), closedir()
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Arguments missing.\n");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        const char *dir_name = argv[i];
        
        // Add a colon to the directory name printout
        printf("%s:\n", dir_name);
        
        // Check if the path itself is a file (not a directory)
        struct stat path_stat;
        if (stat(dir_name, &path_stat) == 0 && !S_ISDIR(path_stat.st_mode)) {
            // If it's a file, just print its name without trying to open as directory
            continue;
        }
        
        DIR *dir = opendir(dir_name);
        
        if (!dir) {
            perror("opendir");
            continue; 
        }

        errno = 0; // vynukujeme errno pred volanim readdir
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            printf("%s\n", entry->d_name);
        }

        // Ak sa vyskytla chyba pri readdir(), vypiseme ju
        if (errno != 0) {
            perror("readdir");
        }

        if (closedir(dir) == -1) {
            perror("closedir");
        }
    }

    return 0;
}