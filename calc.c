#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE    1024

/*  rmspaces: simple helper function for calc which removes the spaces from a line of input
 *  Preconditions:
 *  - line is a null-terminated string
 *
 *  Postconditions:
 *  - line is returned with whitespace removed
 * */
char * rmspaces(char * line){
    int j, l;
    l = strlen(line)+1;
    char * output;
    output = (char *)calloc(l, sizeof(char));
    j = 0;
    for(int i = 0; i<l;i++){
        if(line[i] != ' ' && line[i] != '\t'){
            output[j] = line[i];
            j++;
        }
    }
    output[j] = '\0';
    return output;
}

/*  Calc: Simple integer calculator which uses standard I/O to perform integer arithmetic
 *  Prints the result of simple two argument arithmetic (ex: x OP y where OP is +, -, * or /)
 *
 *  Preconditions:
 *  - standard I/O
 * */
int main(int argc, char** argv){
    char input[MAX_LINE];
    int x, y, numArgs;
    char op;
    while (fgets(input, MAX_LINE, stdin)) {
        numArgs= sscanf(rmspaces(input), "%d%c%d", &x, &op, &y);
        if (numArgs ==3) {
            switch (op) {
                case '+' :
                    printf("%d %c %d = %d\n", x, op, y, (x + y));
                    break;
                case '-' :
                    printf("%d %c %d = %d\n", x, op, y, (x - y));
                    break;
                case '*' :
                    printf("%d %c %d = %d\n", x, op, y, (x * y));
                    break;
                case '/' :
                    if (y > 0) {
                        printf("%d %c %d = %d\n", x, op, y, (x / y));
                        break;
                    } else {
                        printf("Invalid input: dividing by zero is not allowed\n");
                        break;
                    }
                default:
                    printf("Invalid input (Must be in the format x op y where op = +, -, *, /)\n");
                    break;
            }
        } else printf("Too few arguments (Must be in the format x op y where op = +, -, *, /)\n");
    }
    exit(0);
}
