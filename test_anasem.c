#include <stdio.h>
#include "anasem.c"

int main() {
    const char *test_expression = "(q12 ∧ r5) ⇒ (¬a ∧ c)";
    printf("Analyzing expression: %s\n", test_expression);

    // 1. Lexical analysis
    LexemeList lexeme_list = analyseur_lexical(test_expression);
    printf("List of lexemes: ");
    print_lexeme_list(&lexeme_list);

    // 2. Syntax analysis
    printf("Constructing syntax tree...\n");
    Node *syntax_tree = analyseur_syntaxique(&lexeme_list);
    printf("Syntax tree:\n");
    print_tree(syntax_tree, 0);

    // 3. Export syntax tree to .dot file
    const char *dot_file = "syntax_tree.dot";
    export_tree_to_dot(syntax_tree, dot_file);

    // 4. Semantic analysis
    printf("Performing semantic analysis...\n");
    Node *verified_tree = analyseur_semantique(syntax_tree);
    printf("Verified syntax tree:\n");
    print_tree(verified_tree, 0);

    // Free memory
    destroy_tree(verified_tree);
    return 0;
}