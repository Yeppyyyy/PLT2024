#ifndef COMPILATEUR_H
#define COMPILATEUR_H

#include "anasynt.h"  // 包含 Node 和 LexemeList 的定义

#define STACK_SIZE 100
#define PROCESSOR_SIZE 20
#define MAX_SYMBOLS 50
#define MAX_PROGRAM_SIZE 100

// 栈结构定义
typedef struct {
    int data[STACK_SIZE];
    int top;
} Stack;
// 符号表结构定义
typedef struct {
    char name[10];
    int index;  // 变量对应的索引 (-1, -2, ...)
} Symbol;

typedef void (*base)(void);  

// 全局变量声明
extern Stack stack;                          // 全局栈  // 符号表
extern int symbol_count;                     // 符号计数
extern Symbol symbol_table[MAX_SYMBOLS]; 
extern base processor[PROCESSOR_SIZE];

// 栈操作函数
void init_stack(Stack *s);
void push(Stack *s, int value);
int pop(Stack *s);

// 获取或添加变量到符号表
int get_or_add_symbol(const char *name);

// 从语法树生成虚拟机代码
void compile_from_syntax_tree(Node *root, int *program, int *size);

// 初始化处理器
void init_processor(void);

// 逻辑运算符函数
void Non(void);
void Et(void);
void Ou(void);
void Implique(void);

#endif // COMPILATEUR_H
