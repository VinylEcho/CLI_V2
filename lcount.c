#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE    1024
#define FIRST_CHAR argv[i][0]

/*  lcount: takes in the name(s) of 1 or more files as arguments, then counts how many times each character appears
 *  Preconditions:
 *  - argv contains 0 or more file names
 *  - reads in a line from stdin if no files are specified
 *
 *  Postconditions:
 *  - prints total occurrences of each character in all the given files
 *  - if one or more files can't be opened, prints an error then aborts
 * */
int main(int argc, char** argv){
    int letters[26] = {0};
    int i = 1;
    int c = 0;
    char input[MAX_LINE];
    FILE *fp;
    // Reads from stdin if there are
    if(argv[1]==NULL) {
        i = 0;
        while ((fread(input,1,1,stdin))) {
            c = input[i];
            c = tolower(c);
            if (c >= 'a' && c <= 'z') {
                letters[(c - 'a')]++;
            }
        }
    }else {
        while (argv[i] != NULL) {
            // Attempts to open each file passed in through the command line
            fp = fopen(argv[i], "r");
            if (fp != NULL) {
                while ((c = fgetc(fp)) != EOF) {
                    c = tolower(c);
                    if (c >= 'a' && c <= 'z') {
                        letters[(c - 'a')]++;
                    }
                }
                fclose(fp);
                i++;
                // Prints an error and aborts if any given file could not be opened
            } else {
                printf("Error: could not open file %s\n", argv[i]);
                exit(0);
            }
        }
    }

    for(int j = 0; j<26;j++) printf("%c:%d ", (j+'a'), letters[j]);
    printf("\n");
    exit(0);
}