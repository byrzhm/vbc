#pragma once

#include <stdbool.h>
#include <stdint.h>

//-- lexer -----

enum {
    TK_NOTYPE = 256,
    TK_UMINUS,        // unary minus -
    TK_EQ,            // ==
    TK_NEQ,           // !=
    TK_AND,           // &&
    TK_DEC,           // decimal
    TK_HEX,           // hexadecimal
};


#define TK_LEN_MAX 64
#define TK_NR_MAX  128

typedef struct token {
    int type;
    char str[TK_LEN_MAX];
} Token;

extern Token tokens[TK_NR_MAX];
extern int nr_token;

void init_regex();
bool make_token(char *);

//-- parser -----

uint32_t parse(char *, bool *);

