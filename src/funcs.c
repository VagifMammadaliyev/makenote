#include <stdio.h>
#include <stdlib.h>

void eject(char *message) {
    char *usage = 
        "Usage: \n\tmakenote 'note' [-b notebook] [-i]\
        \n\tmakenote -r [a/f/l/i] [-b notebook]\
        \n\tmakenote -c [-b notebook]\n";
    printf("Error: %s\n%s", message, usage);
    exit(1);
}

