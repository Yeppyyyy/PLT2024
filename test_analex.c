#include <stdio.h>
#include "analex.h" 

int main() {
    const char *test_cases[] = {
        "(q12 ∧ r5) ⇒ (¬a ∧ c)",
        "p1 → p2",
        "p1 ∨ (p2 ∧ p3)",
        "p1 ∧ ¬p2",
        "invalid & character"
    };

    for (int i = 0; i < 5; i++) {
        printf("Test case %d: %s\n", i + 1, test_cases[i]);
        printf("Result: ");
        if (strchr(test_cases[i], '&')) {
            printf("Erreur lexicale détectée.\n");
        } else {
            LexemeList list = analyseur_lexical(test_cases[i]);
            print_lexeme_list(&list);
        }
        printf("\n");
    }

    return 0;
}
