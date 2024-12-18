#ifndef ANALEX_H
#define ANALEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Macro definitions
#define MAX_LEXEMES 100
#define MAX_LEXEME_LEN 50

// Lexeme structure
typedef struct {
    char type[20];            // "PO", "PF", "Prop", "Op"
    char value[MAX_LEXEME_LEN];
} Lexeme;

// LexemeList structure
typedef struct {
    Lexeme lexemes[MAX_LEXEMES];
    int count;
} LexemeList;

// Function declarations
void add_lexeme(LexemeList *list, const char *type, const char *value);
int match_regex(const char *str, const char *pattern);
LexemeList analyseur_lexical(const char *expression);
void print_lexeme_list(const LexemeList *list);

#endif // ANALEX_H
