#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "notebook.h"

int parse_args(char **args, int argc,  int *im, int *r, char **n, char **b, int *c) {
    int i;
    char *arg;
    char *value;

    for (i = 0; i < argc; i++) {
        arg = args[i];
        if (arg[0] == '-' && arg[2] == '\0') {
            switch (arg[1]) {
                case 'i':
                    *im = 1;
                    break;
                case 'r':
                    value = args[++i];         
                    if (value) {
                        if (strcmp(value, "a") == 0) *r = ALL;
                        else if (strcmp(value, "f") == 0) *r = FIRST;
                        else if (strcmp(value, "l") == 0) *r = LAST;
                        else if (strcmp(value, "i") == 0) *r = IMPORTANT;
                        else return INVALID_VALUE;
                    } else return NO_VALUE_PROVIDED;
                    break;
                case 'b':
                    *b = args[++i];
                    if (!*b || (*b)[0] == '-') return NO_VALUE_PROVIDED;
                    break;
                case 'c':
                    *c = 1;
                    *b = args[++i];
                    if (*b && (*b)[0] == '-') return INVALID_VALUE;
                    break;
                default:
                    return INVALID_OPTION;
            }
        } else if (arg[0] == '-' && arg[2] != '\0'){
            return INVALID_OPTION;
        } else {
            if (*n) return NOTE_PROVIDED_TWICE;
            *n = arg;
        }
    }
   
    // check for unsuitable arguments
    if (
        (*n && (*c || *r)) || 
        (*c && (*r || *n || *im)) || 
        (*r && (*c || *n || *im)) ||
        (*b && !(*c || *n || *r))
    ) {
        return INVALID_MIX;
    }

    return 0;
}
