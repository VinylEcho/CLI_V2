/*  History
 *  Provides methods to:
 *  - Store static history of commands
 *  - Read
 * */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "ttymode.h"
#include "history.h"

/*  addcmd: stores the selected command in the list of most recently used commands
 *
 *  Preconditions:
 *  - None
 *
 *  Postconditions:
 *  - History stores up to MAX_HISTORY entries in chronological order
 * */
void addCmd (char * cmd){
    for(int i = MAX_HISTORY-1; i>0; i--) if(history[i-1] != NULL)strcpy(history[i], history[i-1]);
    if(total < MAX_HISTORY) total++;
    strcpy(history[0], cmd);
}


/*  historySetup: fills the history array with null strings
 *  Preconditions:
 *  - none
 *  Postconditions:
 *  - history is filled with null strings
 *  - total is set to 0
 *  - tty_atexit is registered to be run when the CLI quits
 * */
void historySetup(){
    atexit(tty_atexit);
    for (int i = 0; i < MAX_HISTORY; i++) history[i] = (char *)calloc((MAX_LINE), sizeof(char));
    total = 0;
}

/*  clearDisplay: simple helper function which overwrites the current line of input
 *  Preconditions:
 *  - None
 *
 *  Postconditions:
 *  - terminal displays a blank line
 * */
void clearDisplay(int l){
    for(int i = l; i >0; i--) printf("\b \b");
}

/*  getHistory: reads in input character by character until the user confirms selection by pressing enter
 *
 *  Preconditions:
 *  - input is a valid character array
 *  - historySetup has been run prior to execution (to initialize variables and register atexit command)
 *
 *  Postconditions:
 *  - Input is filled with a series of user characters
 *  - Returns input, or NULL if user enters EOF
 * */
char * getHistory(char* input){
    char c;
    int i = 0
    int p = 0;
    tty_cbreak(STDIN_FILENO);

    // Runs until the user selects a command with enter, then returns
    while((c = getchar())){
        // Preps input for return by adding a null character and adds input to the command history if it contains any characters
        if(c == '\n') {
            input[i] = '\0';
            if(i>0) addCmd(input);
            break;
        } else if(c == 4){
            // Clears input buffer if EOF is entered and there is buffered input, else returns EOF
            input = NULL;
            break;
        // Simple backspace method, replaces most recent character with a space
        }else if (c == 8 || c == 127){
            if(i > 0) {
                printf("\b \b");
                p = 0;
                i--;
            }
        // Looks for the start of an arrow sequence, displaying history if an up or down arrow is detected
        } else if(c == 27){
            getchar();
            c = getchar();
            // Up arrow method, cycles forward through history until it reaches the oldest command
            if(c == 65){
                clearDisplay(i);
                strcpy(input, history[p]);
                if(p < total-1) p++;
                i = strlen(input);
                printf("%s", input);
            // Down arrow method, cycles backwards through history, or clears input if already at the beginning of history
            } else if (c == 66){
                if(p > 0) p--;
                clearDisplay(i);
                if(p > 0) {
                    strcpy(input, history[p]);
                    i = strlen(input);
                    printf("%s", input);
                } else i = 0;
            }
        }else{
            // Prevents buffer overflow by only adding characters while the buffer has room
            if(i < MAX_LINE) {
                printf("%c", c);
                input[i] = c;
                p = 0;
                i++;
            }
        }
    }
    tty_reset(STDIN_FILENO);
    printf("\n");
    return input;
}