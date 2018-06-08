/* Tokenizer: Takes in a string and splits the list into a list of tokens by replacing spaces and double quotes with null characters.
    Returns an array of pointers to each token
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tokenizer.h"
#define CURRENT (line[tokenStart])
#define TOKEN_END (line[tokenEnd])


/*  tokencounter: Helper function for gettokens which counts how many non null tokens are in the input string
 *  Preconditions:
 *  - line is a valid, null terminated array (aka a string)
 *
 *  Postconditions:
 *  - tokens = number of non-null substrings (ex: line = "tom \"\" servo" will return a value of 2)
 * */
int tokencounter(char * line){
    int i, tokens, tokenStart;
    i = tokens = 0;
    while (line[i] != '\0'){
        if(line[i] != ' ' && line[i] != '\t'){
            if (line[i] == '\"'){
                i++;
                tokenStart = i;
                // Increments through the array until a closing quote is found, breaking early if the string ends before a closing quote is found
                while(line[i] != '\"' && line[i] != '\0') {
                    i++;
                }
                if((i - tokenStart) >0) tokens++;
                i++;
            } else{
                tokens++;
                do{
                    i++;
                    if(line[i] == ' ' || line[i] == '\t'){
                        break;
                    } else if(line[i] == '\"'){
                        break;
                    }
                } while(line[i] != '\0');
            }
        } else i++;
    }
    return tokens;
}

/*  gettokens: parses through the input line and returns an array of tokens split on whitespace or quotes
 *  Preconditions:
 *  - line is a valid null terminated array of characters
 *
 *  Postconditions:
 *  - Tokens contains an array of substrings split by whitespace or double quotes
 *  - Example 1: the input string "tom servo" will return two tokens: "tom" and "servo"
 *  - Example 2: the input string "\"tom servo\"" will return one token: "tom servo"
 * */
char **gettokens(char *line){
    int j, s, t, len, tokenStart, tokenEnd;
    s = strlen(line);
    t = tokencounter(line);
    // Allocates space for the copied tokens on the stack
    char **tokens;
    tokens = (char **)calloc((t+1), sizeof(char*));
    for (int i = 0; i < t+1; ++i) tokens[i] = (char *)calloc((s+1), sizeof(char));

    j = len = tokenStart = tokenEnd = 0;
    while(CURRENT != '\0'){
        if(CURRENT == ' ' || CURRENT == '\t') tokenStart++;
        else{
            // Parses along a substring until a null character or closing quote is found.
            // Substring length is calculated, then the counter is advanced to skip the closing quote
            if(CURRENT =='\"') {
                tokenStart++;
                tokenEnd = tokenStart;
                while (TOKEN_END != '\"' && TOKEN_END != '\0') {
                    tokenEnd++;
                }
                len = tokenEnd - tokenStart;
                tokenEnd++;
            // Parses along a substring until a null character, double quote, or whitespace is encountered.
            } else{
                tokenEnd = tokenStart;
                while (TOKEN_END != ' ' && TOKEN_END != '\t' && TOKEN_END != '\0' && TOKEN_END != '\"'){
                    tokenEnd++;
                }
                len = tokenEnd - tokenStart;
            }
            // Adds current token to list of tokens if it's not null (in case of "")
            if(len > 0){
                strncpy(tokens[j], &CURRENT, len);
                tokens[j][(len+1)] = '\0';
                j++;
            }
            tokenStart = tokenEnd;
        }
    }
    tokens[j] = NULL;
    return &tokens[0];
}

/*  redir: searches for a file redirection string, returning the file name after the desired character (typically '>' or '<')
 *
 *  Preconditions:
 *  - tokens is a valid array of one or more strings
 *  - Op is a valid character
 *
 *  Postconditions:
 *  - returns the first string representing the file name for redirection
 *  - if op is not found, returns null
 *  - Readjusts the list of tokens to remove the redirection token and / or symbol (if the redirection has a space between the symbol and filename)
 * */
char *redir(char op, char** tokens){
    int i = 0;
    int j = 0;
    char* output = (char *)calloc((MAX_LINE), sizeof(char));
    while(tokens[i]!=NULL){
        if(tokens[i][0] == op){
            if(tokens[i][1] != '\0') {
                strcpy(output, &tokens[i][1]);
                j = (i+1);
            } else {
                strcpy(output, &tokens[i + 1][0]);
                j = (i+2);
            }
            // Removes the redirection token(s) and shifts the rest of the elements forward
            do{
                tokens[i] = tokens[j];
                i++;
                j++;
            }while (tokens[i]!= NULL);
            return output;
        }
        i++;
    }
    // If no redirection is found, return nothing
    return NULL;
}
