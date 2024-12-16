#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"


typedef struct Node {
    char type[20];           // Node type ("Prop", "Op", "PO", "PF")
    char value[MAX_LEXEME_LEN]; // Node value ("p1", "ET", etc.)
    struct Node *left;       // Left child node
    struct Node *right;      // Right child node
} Node;

// Create a syntax tree node
Node* create_node(const char *type, const char *value) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(node->type, type, sizeof(node->type) - 1);
    strncpy(node->value, value, sizeof(node->value) - 1);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Destroy the syntax tree
void destroy_tree(Node *root) {
    if (!root) return;
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

// Print the syntax tree (for debugging)
void print_tree(Node *root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("[%s: %s]\n", root->type, root->value);
    print_tree(root->left, depth + 1);
    print_tree(root->right, depth + 1);
}

// Main function: construct the syntax tree
Node* parse_expression(LexemeList *list, int *index);

// Parse a factor: an expression in parentheses or a propositional variable
Node* parse_factor(LexemeList *list, int *index) {
    if (*index >= list->count) {
        fprintf(stderr, "Syntax error: incomplete expression.\n");
        exit(EXIT_FAILURE);
    }
    Lexeme *lex = &list->lexemes[*index];

    if (strcmp(lex->type, "Prop") == 0) {
        // Return a propositional variable node
        (*index)++;
        return create_node("Prop", lex->value);
    } else if (strcmp(lex->type, "PO") == 0) {
        // Parse the expression inside parentheses
        (*index)++;
        Node *node = parse_expression(list, index);
        if (*index >= list->count || strcmp(list->lexemes[*index].type, "PF") != 0) {
            fprintf(stderr, "Syntax error: missing closing parenthesis.\n");
            exit(EXIT_FAILURE);
        }
        (*index)++;
        return node;
    } else if (strcmp(lex->type, "Op") == 0 && strcmp(lex->value, "NON") == 0) {
        // Parse the NOT operation
        (*index)++;
        Node *node = create_node("Op", "NON");
        node->right = parse_factor(list, index);
        return node;
    }

    fprintf(stderr, "Syntax error: unexpected token '%s'.\n", lex->value);
    exit(EXIT_FAILURE);
}

// Parse a term: handle the `AND` operation
Node* parse_term(LexemeList *list, int *index) {
    Node *node = parse_factor(list, index);
    while (*index < list->count && strcmp(list->lexemes[*index].value, "ET") == 0) {
        Lexeme *lex = &list->lexemes[*index];
        (*index)++;
        Node *op_node = create_node("Op", lex->value);
        op_node->left = node;
        op_node->right = parse_factor(list, index);
        node = op_node;
    }
    return node;
}

// Parse an expression: handle the `OR` and `IMPLIES` operations
Node* parse_expression(LexemeList *list, int *index) {
    Node *node = parse_term(list, index);
    while (*index < list->count) {
        Lexeme *lex = &list->lexemes[*index];
        if (strcmp(lex->value, "OU") == 0 || strcmp(lex->value, "IMPLIQUE") == 0) {
            (*index)++;
            Node *op_node = create_node("Op", lex->value);
            op_node->left = node;
            op_node->right = parse_term(list, index);
            node = op_node;
        } else {
            break;
        }
    }
    return node;
}

// Syntax analysis function interface
Node* analyseur_syntaxique(LexemeList *list) {
    int index = 0;
    Node *root = parse_expression(list, &index);
    if (index < list->count) {
        fprintf(stderr, "Syntax error: remaining tokens not analyzed.\n");
        exit(EXIT_FAILURE);
    }
    return root;
}

// Export the syntax tree to a Graphviz .dot file
void export_tree_to_dot(Node *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: unable to open file '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "digraph SyntaxTree {\n");
    fprintf(file, "    node [shape=ellipse];\n");

    // Recursively write nodes and edges
    int node_id = 0;

    void write_node(Node *node, int *id) {
        if (!node) return;

        int current_id = (*id)++;
        fprintf(file, "    node%d [label=\"%s\\n%s\"];\n", current_id, node->type, node->value);

        if (node->left) {
            int left_id = *id;
            write_node(node->left, id);
            fprintf(file, "    node%d -> node%d;\n", current_id, left_id);
        }
        if (node->right) {
            int right_id = *id;
            write_node(node->right, id);
            fprintf(file, "    node%d -> node%d;\n", current_id, right_id);
        }
    }

    write_node(root, &node_id);
    fprintf(file, "}\n");
    fclose(file);

    printf("Tree exported to '%s'.\n", filename);
    printf("You can generate a graph with: dot -Tpng %s -o tree.png\n", filename);
}