#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anasynt.c"

// Check if the node is an operator
int is_operator(const char *type) {
    return strcmp(type, "Op") == 0;
}

// Check if the node is a proposition variable
int is_proposition(const char *type) {
    return strcmp(type, "Prop") == 0;
}

// Semantic check function
void check_semantics(Node *node) {
    if (!node) return;

    // If the node is an operator, check its children
    if (is_operator(node->type)) {
        if (strcmp(node->value, "NON") == 0) {
            // "NON" must have only one child (right child)
            if (!node->right || node->left) {
                fprintf(stderr, "Semantic error: 'NON' must have one operand.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            // Other operators (like ET, OU, IMPLIQUE) must have two children
            if (!node->left || !node->right) {
                fprintf(stderr, "Semantic error: operator '%s' must have two operands.\n", node->value);
                exit(EXIT_FAILURE);
            }
        }
    } else if (is_proposition(node->type)) {
        // If the node is a proposition variable, it cannot have children
        if (node->left || node->right) {
            fprintf(stderr, "Semantic error: proposition '%s' cannot have children.\n", node->value);
            exit(EXIT_FAILURE);
        }
    } else {
        // Unknown node type
        fprintf(stderr, "Semantic error: unknown node type '%s'.\n", node->type);
        exit(EXIT_FAILURE);
    }

    // Recursively check children nodes
    check_semantics(node->left);
    check_semantics(node->right);
}

// Semantic analyzer interface
Node* analyseur_semantique(Node *syntax_tree) {
    if (!syntax_tree) {
        fprintf(stderr, "Error: empty syntax tree.\n");
        exit(EXIT_FAILURE);
    }

    // Check the semantics of the syntax tree
    check_semantics(syntax_tree);

    // If semantics are correct, return the syntax tree
    return syntax_tree;
}