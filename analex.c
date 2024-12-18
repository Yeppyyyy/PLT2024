#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_LEXEMES 100
#define MAX_LEXEME_LEN 50

typedef struct {
    char type[20]; // "PO", "PF", "Prop", "Op"
    char value[MAX_LEXEME_LEN];
} Lexeme;

typedef struct {
    Lexeme lexemes[MAX_LEXEMES];
    int count;
} LexemeList;

void add_lexeme(LexemeList *list, const char *type, const char *value) {
    if (list->count >= MAX_LEXEMES) {
        fprintf(stderr, "Erreur: trop de lexèmes.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(list->lexemes[list->count].type, type, sizeof(list->lexemes[list->count].type) - 1);
    strncpy(list->lexemes[list->count].value, value, sizeof(list->lexemes[list->count].value) - 1);
    list->count++;
}

int match_regex(const char *str, const char *pattern) {
    regex_t regex;
    int result;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Erreur: compilation de l'expression régulière.\n");
        exit(EXIT_FAILURE);
    }
    result = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);

    return result == 0;
}

LexemeList analyseur_lexical(const char *expression) {
    LexemeList list = {.count = 0};
    const char *ptr = expression;

    while (*ptr) {
        if (*ptr == ' ') {
            ptr++;
            continue;
        }
        if (*ptr == '(') {
            add_lexeme(&list, "PO", "(");
            ptr++;
        } else if (*ptr == ')') {
            add_lexeme(&list, "PF", ")");
            ptr++;
        } else if (match_regex(ptr, "^→")) {
            add_lexeme(&list, "PRODUIT", "PRODUIT");
            ptr += strlen("→");
        } else if (match_regex(ptr, "^[a-zA-Z][a-zA-Z0-9]*")) {
            char buffer[MAX_LEXEME_LEN] = {0};
            sscanf(ptr, "%[a-zA-Z0-9]", buffer);
            add_lexeme(&list, "Prop", buffer);
            ptr += strlen(buffer);
        } else if (match_regex(ptr, "^∧")) {
            add_lexeme(&list, "Op", "ET");
            ptr += strlen("∧");
        } else if (match_regex(ptr, "^∨")) {
            add_lexeme(&list, "Op", "OU");
            ptr += strlen("∨");
        } else if (match_regex(ptr, "^⇒")) {
            add_lexeme(&list, "Op", "IMPLIQUE");
            ptr += strlen("⇒");
        } else if (match_regex(ptr, "^¬")) {
            add_lexeme(&list, "Op", "NON");
            ptr += strlen("¬");
        } else {
            fprintf(stderr, "Erreur lexicale: caractère invalide '%c'.\n", *ptr);
            exit(EXIT_FAILURE);
        }
    }
    return list;
}

void print_lexeme_list(const LexemeList *list) {
    for (int i = 0; i < list->count; i++) {
        printf("[%s(\"%s\")]", list->lexemes[i].type, list->lexemes[i].value);
        if (i < list->count - 1) printf(", ");
    }
    printf("\n");
}
