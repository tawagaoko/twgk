#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

typedef struct Token {
    int kind;
    char * text;
    int len;
    int capacity;
} Token;

enum TokenKind {
    T_EOF,
    T_NEWLINE,
    T_OPEN,
    T_CLOSE,
    T_SEQ,
    T_CONJ,
    T_BACK,
    T_DISJ,
    T_PIPE,
    T_IN,
    T_APPEND,
    T_OUT,
    T_WORD
};

int
init_scanner(FILE *);

void
free_scanner(void);

int
next_token(Token *);

void
free_token(Token *);

#endif
