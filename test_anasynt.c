#include <stdio.h>
#include "anasynt.c"

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
    const char *dot_file = "syntax_tree_anasynt.dot";
    export_tree_to_dot(syntax_tree, dot_file);
    printf("The syntax tree file has been exported to '%s'.\n", dot_file);
    printf("Use the following command to generate a graph:\n");
    printf("dot -Tpng %s -o syntax_tree_anasynt.png\n", dot_file);

    // Free memory
    destroy_tree(syntax_tree);
    return 0;
}