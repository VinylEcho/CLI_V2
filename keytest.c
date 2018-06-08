#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ttymode.h"

int main() {
    
    atexit(tty_atexit);
    tty_cbreak(STDIN_FILENO);
    
    char c;
    while (1) {
        c = getchar();
        printf("%d ", c);
    }
    
    
    return 0;
}