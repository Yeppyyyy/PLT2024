#include <stdio.h>
#include "compilateur.h"
#include "anasynt.h"
#include "analex.h"
int main() {
    // 初始化栈和处理器
    init_stack(&stack);
    init_processor();

    // Step 1: 词法分析
    printf("Starting lexical analysis...\n");
    char *word = "((q12 ∧ r5) ∨ ((¬a) ∧ c))⇒ (r5 ⇒ c)";
    LexemeList lexeme_list = analyseur_lexical(word);
    printf("List of lexemes: ");
    print_lexeme_list(&lexeme_list);

    // Step 2: 语法树生成
    printf("\nConstructing syntax tree...\n");
    Node *syntax_tree = analyseur_syntaxique(lexeme_list);

    // Step 3: 从语法树编译虚拟机代码
    int program[MAX_PROGRAM_SIZE];
    int program_size = 0;
    compile_from_syntax_tree(syntax_tree, program, &program_size);

    // Step 4: 输出符号表
    printf("\nSymbol Table:\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("Variable: %s, Index: %d\n", symbol_table[i].name, symbol_table[i].index);
    }

    // Step 5: 输出虚拟机指令
    printf("\nCompiled Program:\n");
    for (int i = 0; i < program_size; i++) {
        printf("%d ", program[i]);
    }
    printf("\n");

    return 0;
}
