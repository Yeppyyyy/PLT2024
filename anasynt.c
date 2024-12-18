#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"

// Lexeme list
LexemeList lexeme_list;
// Current position
int current_pos;

// Node structure for the syntax tree
typedef struct Node {
    char type[20];               // Node type ("Prop", "Op", etc.)
    char value[MAX_LEXEME_LEN];  // Node value ("p1", "ET", etc.)
    struct Node *left;           // Left child node
    struct Node *right;          // Right child node
} Node;

// Function declarations
Node* factor();
Node* term();
Node* expression();
void print_tree(Node *root, int depth);
void destroy_tree(Node *root);

// Utility function: Get the current lexeme
Lexeme current_token() {
    if (current_pos >= 0 && current_pos < lexeme_list.count) {
        return lexeme_list.lexemes[current_pos];
    } else {
        Lexeme eof = {"EOF", ""};
        return eof;
    }
}

// Consume the current lexeme and move to the previous one
void consume() {
    if (current_pos >= 0) {
        current_pos--;
    }
}

// Create a new syntax tree node
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

// Parse <factor>
Node* factor() {
    Lexeme t = current_token();
    Node *node = NULL; 

    if (strcmp(t.type, "Prop") == 0) { // Handle PROP
        node = create_node("Prop", t.value);
        consume(); // Consume PROP
    } 
    else if (strcmp(t.type, "PF") == 0) { // Handle parentheses
        consume(); // Consume ")"
        node = term();
        if (strcmp(current_token().type, "PO") != 0) {
            fprintf(stderr, "Error: Missing left parenthesis at position %d\n", current_pos);
            exit(EXIT_FAILURE);
        }
        consume(); // Consume "("
    } 
    else {
        fprintf(stderr, "Error: Invalid factor at position %d\n", current_pos);
        exit(EXIT_FAILURE);
    }

    while (strcmp(current_token().value, "NON") == 0) { // Handle "NON" operator
        consume(); // Consume "NON"
        Node *node1 = create_node("Op", "NON");
        node1->right = node; // "NON" is unary, attach the right node
        node = node1; 
    }

    return node; 
}

// Parse <term>
Node* term() {
    Node *node = factor();

    if (strcmp(current_token().type, "Op") == 0 && strcmp(current_token().value, "NON") != 0) {
        Lexeme t = current_token();
        consume();
        Node *op_node = create_node("Op", t.value);        
        op_node->left = term();
        op_node->right = node;
        node = op_node;
    }
    return node;
}

// Parse <expression>
Node* expression() {
    Node *node = term();

    if (strcmp(current_token().type, "PRODUIT") == 0 ) {
        Lexeme t = current_token();
        consume();
        Node *op_node = create_node("Op", t.value);
        op_node->left = term();
        op_node->right = node;
        node = op_node;
    }
    return node;
}

// Print the syntax tree (for debugging)
void print_tree(Node *root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("[%s: %s]\n", root->type, root->value);
    print_tree(root->left, depth + 1);
    print_tree(root->right, depth + 1);
}

// Destroy the syntax tree and free memory
void destroy_tree(Node *root) {
    if (!root) return;
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

// Run function: Check if input conforms to the grammar
int run(LexemeList list) {
    lexeme_list = list;
    current_pos = lexeme_list.count - 1; // Start from the end (right-to-left parsing)

    // Test if input conforms to the grammar
    expression();

    // Verify if all lexemes are consumed
    if (strcmp(current_token().type, "EOF") != 0 && current_pos != -1) {
        fprintf(stderr, "Error: Unparsed extra characters\n");
        return EXIT_FAILURE;
    }

    printf("Parsing completed: Syntax is correct!\n");
    return 0;
}

// Generate the syntax tree
Node* analyseur_syntaxique(LexemeList list) {
    lexeme_list = list;
    current_pos = lexeme_list.count - 1;
    return expression();
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

