/*  History.h
 *
 *  Provides methods to store a command history and interpret keystrokes
 * */

// Takes in a pointer to an array, then saves selected input to the supplied array
#define MAX_HISTORY 10
#define BACKSPACE '\b'
// Stores a list of up to 10 previous entries
char * history[MAX_HISTORY];
int total;

void historySetup();
char * getHistory(char* input);