#include <stdio.h>
#include "anasem.h"

int main() {
    // Define test expressions
    const char *test_cases[] = {
        "(p1 ⇒ p2) → ((¬p1) ∨ p2)",
        "(p1 ∨ (p2 ∧ p3)) → ((p1 ∨ p2) ∧ (p1 ∨ p3))",
        "(p1 ∨ p2) → (p2 ∨ p1)",
        "(p1 ∧ p2) → (p2 ∧ p1)",
        "¬(p1 ∧ p2) → ((¬p1) ∨ (¬p2))",
        "¬(p1 ∨ p2) → ((¬p1) ∧ (¬p2))",
        "¬(¬p1) → p1",
        "¬((p1 ∧ p2) ∨ (¬p3 ∧ p4)) → ((¬p1 ∨ ¬p2) ∧ (p3 ∨ ¬p4))"

    };

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_tests; i++) {
        printf("\n========================\n");
        printf("Test Case %d: %s\n", i + 1, test_cases[i]);

        // 1. Lexical analysis
        LexemeList lexeme_list = analyseur_lexical(test_cases[i]);
        //printf("List of lexemes: ");
        //print_lexeme_list(&lexeme_list);

        // 2. Syntax analysis
        //printf("Constructing syntax tree...\n");
        Node *syntax_tree = analyseur_syntaxique(lexeme_list);
        //printf("Syntax tree:\n");
        //print_tree(syntax_tree, 0);

        //3. Semantic analysis
        printf("Performing semantic analysis...\n");
        Node *verified_tree = analyseur_semantique(syntax_tree);
        if (verified_tree != NULL) {
            printf("Verified syntax tree:\n");
            //print_tree(verified_tree, 0);
            printf("Semantic analysis passed.\n");
        } else {
            printf("Semantic analysis failed.\n");
        }
    }

    printf("\nAll test cases completed.\n");
    return 0;
}
