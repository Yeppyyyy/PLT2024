#include <stdio.h>
#include "anasynt.h"  // Assuming required headers and function declarations are here

int main() {
    // Test cases
    const char *test_cases[] = {
        "(q12 ∧ r5) ⇒ (¬a ∧ c)",
        "p1 → p2",
        "p1 ∨ (p2 ∧ p3)",
        "p1 ∧ ¬p2",
        "(q12 ∧ r5) ⇒ (¬¬(¬a) ∧ c)"
    };

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    // Loop through all test cases
    for (int i = 0; i < num_tests; i++) {
        printf("===== Test Case %d =====\n", i + 1);
        printf("Analyzing expression: %s\n", test_cases[i]);

        // Step 1: Lexical analysis
        LexemeList lexeme_list = analyseur_lexical(test_cases[i]);
        printf("List of lexemes: ");
        print_lexeme_list(&lexeme_list);

        // Step 2: Syntax tree generation
        printf("Constructing syntax tree...\n");
        Node *syntax_tree = analyseur_syntaxique(lexeme_list);
        printf("Syntax tree:\n");
        print_tree(syntax_tree, 0);

        // Step 3: Export syntax tree to .dot file
        char dot_file[50];
        snprintf(dot_file, sizeof(dot_file), "syntax_tree_test%d.dot", i + 1);
        export_tree_to_dot(syntax_tree, dot_file);
        printf("The syntax tree file has been exported to '%s'.\n", dot_file);
        printf("Use the following command to generate a graph:\n");
        printf("dot -Tpng %s -o syntax_tree_test%d.png\n", dot_file, i + 1);

        // Step 4: Free memory
        destroy_tree(syntax_tree);
        printf("Memory for test case %d has been freed.\n\n", i + 1);
    }

    printf("All test cases completed successfully.\n");
    return 0;
}
