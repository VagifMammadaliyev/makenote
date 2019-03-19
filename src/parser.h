#ifndef PARSER_H
#define PARSER_H

enum Errors {
    INVALID_OPTION=1,
    NOTE_PROVIDED_TWICE,
    INVALID_VALUE,
    NO_VALUE_PROVIDED,
    INVALID_MIX
};


int parse_args(char **args, int argc, int *i, int *r, char **n, char **b, int *c);

#endif
