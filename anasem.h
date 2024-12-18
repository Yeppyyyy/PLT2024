#ifndef ANASYNT_H
#define ANASYNT_H
#include "anasynt.h" 
// Function prototypes
Node* copy_tree(Node *node);
void swap_children(Node *node);
int are_trees_identical(Node *tree1, Node *tree2);
int compare_semantic(Node *target, Node *variable);
Node* analyseur_semantique(Node *tree);

#endif // ANASYNT_H
