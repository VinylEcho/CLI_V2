/* tokenizer.h

    header file for tokenizer
    breaks a string into substrings separated by spaces
    substrings enclosed in double quotes may contain spaces
    
*/

#define MAX_LINE    1024

// gettokens returns an array of strings
char **gettokens (char *line);
char *redir(char op, char** tokens);