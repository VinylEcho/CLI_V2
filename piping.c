#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "CLI.h"
#include "tokenizer.h"

/*  pipeCheck: takes in a list of tokens and checks if there is a redirection token ("|")
 *
 *  Preconditions:
 *  - tokens is an array of one or more strings
 *
 *  Postconditions:
 *  - returns 0 if no redirection token is found, or the index of the redirection token if one is found
 * */
int pipeCheck(char ** tokens){
    int index = 0;
    int i = 0;
    while(tokens[i]!=NULL){
        if(strcmp(tokens[i], "|") == 0) index = i;
        i++;
    }
    return index;
}

/*  len: counts how many tokens there are between index i and the concluding null token
 *
 *  Preconditions:
 *  - i is a valid index for tokens
 *
 *  Postconditions:
 *  - returns the number of tokens between i and the end of tokens
 * */
int len(int i, char ** tokens){
    int end = i;
    while (tokens[end] != NULL) end++;
    return end;
}

void tokenBuilder(char ** tokens, char ** destTokens,  int upper, int offset){
    int i;
    for (i = 0; i<upper; i++) destTokens[i] = tokens[i + offset];
    destTokens[i] = NULL;
}

/*  pipeHelper: creates two sets of tokens and opens a pipe between the two processes
 *
 *  Preconditions:
 *  - tokens is a list of strings
 *  - check is the index of the pipe character
 *
 *  Postconditions:
 *  - The two tokens are piped
 * */
void pipeHelper(char ** tokens, int check){
    int l;
    pid_t pid;
    int fd[2];
    // Copies over the tokens up to the pipe symbol, then caps with a null string
    char *token1[check];
    tokenBuilder(tokens, token1, check, 0);

    // Copies over the tokens just after the pipe symbol to the ending null string
    check++;
    l = len(check, tokens);
    char *token2[(l - check)];
    tokenBuilder(tokens, token2, l-check, check);

    pipe(fd);
    pid = fork();
    if (pid >= 0) {
        if (pid == 0) { // Child
            close(fd[0]);
            if (freopen(redir('<', token1), "r", stdin) == NULL) printf("Redirection failed\n");
            if (fd[1] != STDOUT_FILENO)
            {
                if ((dup2(fd[1], STDOUT_FILENO)) != STDOUT_FILENO)
                {
                    printf("dup2 error to stdout\n");
                    close(fd[1]);
                }
            }
            if (forkage(token1) < 0)
                printf("child error \n");
            // Terminates child so that input does not return to CLI
            exit(0);
        } else { // Parent
            close(fd[1]);	//close write end
            int returnStatus;
            waitpid(pid, &returnStatus, 0);
            if (freopen(redir('>', token2), "w", stdout) == NULL) printf("Redirection failed\n");
            if (fd[0] != STDIN_FILENO)
            {
                if ((dup2(fd[0], STDIN_FILENO)) != STDIN_FILENO)
                {
                    printf("dup2 error to stdin\n");
                    close(fd[0]);
                }
            }

            if (forkage(token2) < 0)
                printf("parent error \n");
        }
    } else {
        printf("Error: fork failed\n");
    }
}

/*  piping: handles redirection and piping for the CLI, then passes the appropriate tokens off to be executed
 *
 *  Preconditions:
 *  - startDir is not null
 *  - tokens contains at least one string
 *
 *  Postconditions:
 *  - a pipe has been established between the specified program(s) if requested
 * */
int piping(char ** tokens){
    int check;
    check = pipeCheck(tokens);
    if(check > 0){
        pipeHelper(tokens, check);
    // If no pipe character is detected,
    } else {
        if (freopen(redir('>', tokens), "w", stdout) == NULL) printf("Redirection failed\n");
        if (freopen(redir('<', tokens), "r", stdin) == NULL) printf("Redirection failed\n");
        forkage(tokens);
    }
    return 0;
}