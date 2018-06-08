#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "ttymode.h"
#include "tokenizer.h"
#include "history.h"
#include "piping.h"

#define CURSOR  "$> "

char startDir[MAX_LINE];

/*  cd: Behaves like the default unix command, but also prints a confirmation message after a successful change.
 *  Preconditions:
 *  - Dir is a valid string
 *  Postconditions:
 *  - Current directory is changed to the specified directory
 *  - Prints a success message on successful change, or an error if the directory does not exist
 * */
int cd(char *dir){
    if(chdir(dir) == 0) printf("cwd changed to %s\n", dir);
    else printf("No such directory: %s\n", dir);
    return 0;
}

/*  Pwd: Prints out the current working directory before the user's next line of input
 *  Preconditions:
 *  - none
 *  Postconditions:
 *  - Current directory is displayed before the next line of input (ex: "/<dir> $>")
 * */
int pwd(){
    char currDir[MAX_LINE];
    getcwd(currDir, sizeof(currDir));
    printf("%s ", currDir);
    return 0;
}

/*  forkage: helper method for the CLI to direct program flow based on the tokenized input.
 *  Preconditions:
 *  - startdir is a valid string and tokens is filled with valid tokens
 *
 *  Postconditions:
 *  - The specified program (or function in the case of CD or PWD) is executed
 *  - If exit is called, then the CLI exits without returning to main
 *  - Returns 0 on successful completion, -1 if fork fails
 * */
int forkage(char ** tokens){
    pid_t pid;
    if (strcmp(tokens[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(tokens[0], "cd") == 0) {
        cd(tokens[1]);
    } else if (strcmp(tokens[0], "pwd") == 0) {
        pwd();
    } else if (strcmp(tokens[0], "lcount") == 0) {
        pid = fork();
        // Creates a child process, then child runs lcount while the parent waits until the child finishes
        if (pid >= 0) {
            if (pid == 0) {
                execvp(strcat(startDir, "/lcount"), tokens);
                exit(0);
            } else {
                int returnStatus;
                waitpid(pid, &returnStatus, 0);
            }
        } else {
            printf("Error: fork failed\n");
        }

    } else if (strcmp(tokens[0], "calc") == 0) {
        pid = fork();
        // Creates a child process, then child runs calc while the parent waits until the child finishes
        if (pid >= 0) {
            if (pid == 0) {
                execvp(strcat(startDir, "/calc"), tokens);
                exit(0);
            } else {
                int returnStatus;
                waitpid(pid, &returnStatus, 0);
            }
        } else {
            printf("Error: fork failed\n");
        }
    } else {
        pid = fork();
        // Creates a child process, then child runs the specified program while the parent waits until the child finishes
        if (pid >= 0) {
            if (pid == 0) {
                execvp(tokens[0], tokens);
                exit(0);
            } else {
                int returnStatus;
                waitpid(pid, &returnStatus, 0);
            }
        } else {
            // Prints and returns an error if fork fails for some reason
            printf("Error: fork failed\n");
            return -1;
        }
    }
    return 0;
}

/*  Shell for the Command Line Interpreter.  Reads in user input, then runs the specified program
 *  Preconditions:
 *  - None
 *
 *  Postconditions:
 *  - Exits once an EOF is read (or the user enters the "exit" command)
 *  - Returns 0 on successful termination
 * */
int main(int argc, char** argv){
    char input[MAX_LINE];
    char** tokens;
    //char startDir[MAX_LINE];
    int l;
    getcwd(startDir, sizeof(startDir));
    startDir[MAX_LINE-1] = '\0';
    historySetup();

    printf(CURSOR);
    while (getHistory(input)) {
        l = strlen(input);
        if(l > 1) {
            /*// Replace the '\n' at the end of the line with a '\0'
            input[l - 1] = '\0';*/

            // get tokens in input line
            tokens = gettokens(input);
            // Passes input tokens to helper function to execute as necessary
            piping(tokens);
            // Mitigates a potential memory leak
            free(tokens);
            freopen("/dev/tty", "w", stdout);
            freopen("/dev/tty", "r", stdin);
        }
        printf(CURSOR);
    }
    printf("\n");
    exit(0);
}