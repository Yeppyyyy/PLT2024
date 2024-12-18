#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include "analex.h"  // 词法分析器头文件，包含 Lexeme 和 LexemeList 结构体

// Node structure for the syntax tree
typedef struct Node {
    char type[20];               // Node type ("Prop", "Op", etc.)
    char value[MAX_LEXEME_LEN];  // Node value ("p1", "ET", etc.)
    struct Node *left;           // Pointer to the left child
    struct Node *right;          // Pointer to the right child
} Node;

// Function declarations

// Create a syntax tree node
Node* create_node(const char *type, const char *value);

// Parse <factor> ::= "NON" <factor> | PROP | ")" <expression> "("
Node* factor();

// Parse <term> ::= <factor> | "Op" <term>
Node* term();

// Parse <expression> ::= <term> | "IMPLIQUE" <expression>
Node* expression();

Node* analyseur_syntaxique(LexemeList list);

// Print the syntax tree (for debugging purposes)
void print_tree(Node *root, int depth);

// Destroy the syntax tree and free memory
void destroy_tree(Node *root);

// Run the parser and build the syntax tree
int run(LexemeList list);

void export_tree_to_dot(Node *root, const char *filename) ;
#endif // SYNTAX_TREE_H
