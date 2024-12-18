#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compilateur.h"

// 栈全局变量
Stack stack;

// 处理器函数指针数组
base processor[PROCESSOR_SIZE];

// 符号表
Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

// 栈操作实现
void init_stack(Stack *s) { s->top = -1; }
void push(Stack *s, int value) {
    if (s->top >= STACK_SIZE - 1) {
        printf("Error: Stack overflow!\n");
        exit(1);
    }
    s->data[++s->top] = value;
}
int pop(Stack *s) {
    if (s->top == -1) {
        printf("Error: Stack underflow!\n");
        exit(1);
    }
    return s->data[s->top--];
}

// 处理器初始化
void init_processor() {
    processor[0] = &Non;
    processor[1] = &Ou;
    processor[2] = &Et;
    processor[3] = &Implique;
}

// 逻辑运算符函数实现
void Non(void) {
    int a = pop(&stack);
    push(&stack, !a);
}

void Et(void) {
    int a = pop(&stack);
    int b = pop(&stack);
    push(&stack, a && b);
}

void Ou(void) {
    int a = pop(&stack);
    int b = pop(&stack);
    push(&stack, a || b);
}

void Implique(void) {
    int a = pop(&stack);
    int b = pop(&stack);
    push(&stack, !b || a);
}

// 符号表操作实现
int get_or_add_symbol(const char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].index;  // 返回已有索引
        }
    }
    if (symbol_count >= MAX_SYMBOLS) {
        printf("Error: Symbol table overflow!\n");
        exit(1);
    }
    symbol_table[symbol_count].index = -(symbol_count + 1); // 新分配索引
    strcpy(symbol_table[symbol_count].name, name);
    return symbol_table[symbol_count++].index;
}

// 从语法树生成虚拟机代码
void compile_from_syntax_tree(Node *root, int *program, int *size) {
    if (root == NULL) return;

    if (strcmp(root->type, "Prop") == 0) {  // 变量节点
        int symbol_index = get_or_add_symbol(root->value);  // 获取或添加符号表索引
        program[(*size)++] = symbol_index;
    } else if (strcmp(root->type, "Op") == 0) { // 操作符节点
        compile_from_syntax_tree(root->left, program, size);   // 遍历左子树
        compile_from_syntax_tree(root->right, program, size);  // 遍历右子树

        // 生成运算符的 Cfa
        if (strcmp(root->value, "NON") == 0) {
            program[(*size)++] = 0;  // NON
        } else if (strcmp(root->value, "OU") == 0) {
            program[(*size)++] = 2;  // OU
        } else if (strcmp(root->value, "ET") == 0) {
            program[(*size)++] = 4;  // ET
        } else if (strcmp(root->value, "IMPLIQUE") == 0) {
            program[(*size)++] = 6;  // IMPLIQUE
        }
    }
}

