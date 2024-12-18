#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anasynt.h"

Node* copy_tree(Node *node) {
    if (!node) return NULL; // Base case: if node is NULL, return NULL

    // Allocate new memory for the node
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Copy data from the original node
    strncpy(new_node->type, node->type, sizeof(new_node->type) - 1);
    strncpy(new_node->value, node->value, sizeof(new_node->value) - 1);

    // Recursively copy left and right children
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);

    return new_node; // Return the new copied node
}
// Swap the left and right children of a node
void swap_children(Node *node) {
    if (!node) return; // If node is NULL, do nothing
    Node *temp = node->left;
    node->left = node->right;
    node->right = temp;
}

// Function to check if two trees are identical
int are_trees_identical(Node *tree1, Node *tree2) {
    // Base cases
    if (!tree1 && !tree2) return 1;  // Both trees are NULL -> identical
    if (!tree1 || !tree2) return 0;  // One tree is NULL -> not identical

    // Check current nodes' type and value
    if (strcmp(tree1->type, tree2->type) != 0 || strcmp(tree1->value, tree2->value) != 0) {
        return 0;  // Current nodes are not equal
    }

    // Recursively check left and right subtrees
    return are_trees_identical(tree1->left, tree2->left) &&
           are_trees_identical(tree1->right, tree2->right);
}



// Compare two syntax trees with semantic analysis
int compare_semantic(Node *target, Node *variable) {
    if (!target && !variable) return 1;  // Both nodes are NULL -> match
    if (!target || !variable) return 0;  // One is NULL, the other is not -> no match

    // If node values are the same, compare subtrees recursively
    if (strcmp(target->value, variable->value) == 0) {//分命题和Operateur
        if ((strcmp(target->value, "OU") == 0) || (strcmp(target->value, "ET") == 0))
            {
            return (compare_semantic(target->left, variable->left) && 
                compare_semantic(target->right, variable->right)) ||
               (compare_semantic(target->left, variable->right) && 
                compare_semantic(target->right, variable->left));
        }
        else{
            return compare_semantic(target->left, variable->left) &&
               compare_semantic(target->right, variable->right);
        }
    }
    else if ((strcmp(target->value, "OU") == 0) && (strcmp(variable->value, "IMPLIQUE") == 0)){
        Node *new_node = create_node("Op", "NON");
        new_node->right = variable->left; //NON 只允许有右子节点
        variable->left = new_node;//不用担心nullptr因为蕴含肯定有东西
        return (compare_semantic(target->left, variable->left) && 
                compare_semantic(target->right, variable->right)) ||
               (compare_semantic(target->left, variable->right) && 
                compare_semantic(target->right, variable->left));

    }//有问题
    else if ((strcmp(target->value, "IMPLIQUE") == 0 )&& (strcmp(variable->value, "OU") == 0)){
        if (strcmp(variable->left->value, "NON") == 0){
            return compare_semantic(target->left, variable->left->right) &&
               compare_semantic(target->right, variable->right);
        } 
        else if(strcmp(variable->right->value, "NON") == 0){
            swap_children(variable);
            return compare_semantic(target, variable);
        } 
    }
    else if ((strcmp(variable->value, "OU") == 0) && (strcmp(target->value, "ET") == 0)){
        if (strcmp(variable->right->value, "ET") == 0 ){
            Node *new_node = create_node("Op", "OU");
            Node *new_node2 = copy_tree(variable->right->left);
            Node *new_node3 = copy_tree(variable->left);
            new_node->left = variable->left;
            new_node->right = new_node2;
            variable->left = new_node;
            strncpy(variable->right->value, "OU", sizeof(variable->right->value) - 1);
            variable->right->value[sizeof(variable->right->value) - 1] = '\0';
            variable->right->left = new_node3; 
            return (compare_semantic(target->left, variable->left) && 
                compare_semantic(target->right, variable->right)) ||
               (compare_semantic(target->left, variable->right) && 
                compare_semantic(target->right, variable->left));
            }
        else if(strcmp(variable->left->value, "ET") == 0 ){
            swap_children(variable);
            return compare_semantic(target,variable);
            }
    }
    else if ((strcmp(variable->value, "ET") == 0) && (strcmp(target->value, "OU") == 0)){
        if ((strcmp(variable->left->value, "OU") == 0 )&&(strcmp(variable->right->value, "OU") == 0 )){
            if (are_trees_identical(variable->left->left, variable->right->left)){
                Node *new_node = copy_tree(variable->left->right);
                variable->left = variable->left->left;
                variable->right->left = new_node;
                strncpy(variable->right->value, "ET", sizeof(variable->right->value) - 1);
                variable->right->value[sizeof(variable->right->value) - 1] = '\0';
                return (compare_semantic(target->left, variable->left) && 
                    compare_semantic(target->right, variable->right)) ||
                    (compare_semantic(target->left, variable->right) && 
                    compare_semantic(target->right, variable->left));
            }
            else if (are_trees_identical(variable->left->right, variable->right->right)){
                swap_children(variable->left);
                swap_children(variable->right);
                return compare_semantic(target,variable);
            }
            else if (are_trees_identical(variable->left->left, variable->right->right)){
                swap_children(variable->right);
                return compare_semantic(target,variable);
            }
            else if (are_trees_identical(variable->left->right, variable->right->left)){
                swap_children(variable->left);
                return compare_semantic(target,variable);
            }

        }
    }
    else if ((strcmp(variable->value, "NON") == 0)&&(strcmp(variable->right->value, "NON") == 0)){
        return compare_semantic(target, variable->right->right);
    }
    else if ((strcmp(target->value, "NON") == 0)&&(strcmp(target->right->value, "NON") == 0)){
        return compare_semantic(target->right->right, variable); 
    }//先简化non有讲究
    else if ((strcmp(variable->value, "NON") == 0) && (strcmp(target->value, "OU") == 0)&&(strcmp(variable->right->value, "ET") == 0)){
        strncpy(variable->value, "OU", sizeof(variable->value) - 1);
        variable->value[sizeof(variable->value) - 1] = '\0';
        Node *new_node = create_node("Op", "NON");
        Node *new_node2 = create_node("Op", "NON");
        new_node->right = variable->right->left;
        new_node2->right = variable->right->right;
        variable->left = new_node;
        variable->right = new_node2;
        return (compare_semantic(target->left, variable->left) && 
                compare_semantic(target->right, variable->right)) ||
               (compare_semantic(target->left, variable->right) && 
                compare_semantic(target->right, variable->left));
    }
    else if ((strcmp(variable->value, "OU") == 0) && (strcmp(target->value, "NON") == 0)){
        strncpy(variable->value, "NON", sizeof(variable->value) - 1);
        variable->value[sizeof(variable->value) - 1] = '\0';
        Node *new_node = create_node("Op", "ET");
        new_node->left = variable->left->right;
        new_node->right = variable->right->right;
        variable->right = new_node;
        return  compare_semantic(target->right, variable->right);
    }
    else if ((strcmp(variable->value, "NON") == 0) && (strcmp(target->value, "ET") == 0)&&(strcmp(variable->right->value, "OU") == 0)){
        strncpy(variable->value, "ET", sizeof(variable->value) - 1);
        variable->value[sizeof(variable->value) - 1] = '\0';
        Node *new_node = create_node("Op", "NON");
        Node *new_node2 = create_node("Op", "NON");
        new_node->right = variable->right->left;
        new_node2->right = variable->right->right;
        variable->left = new_node;
        variable->right = new_node2;
        return (compare_semantic(target->left, variable->left) && 
                compare_semantic(target->right, variable->right)) ||
               (compare_semantic(target->left, variable->right) && 
                compare_semantic(target->right, variable->left));
    }
    else if ((strcmp(variable->value, "ET") == 0) && (strcmp(target->value, "NON") == 0)){
        strncpy(variable->value, "NON", sizeof(variable->value) - 1);
        variable->value[sizeof(variable->value) - 1] = '\0';
        Node *new_node = create_node("Op", "OU");
        new_node->left = variable->left->right;
        new_node->right = variable->right->right;
        variable->right = new_node;
        return  compare_semantic(target->right, variable->right);
    }
    

    return 0; // No matching transformation found
}
// Semantique function: validate the tree
Node* analyseur_semantique(Node *tree) {
    if (!tree) {
        fprintf(stderr, "Error: Tree is NULL.\n");
        return NULL; // Return failure for NULL tree
    }

    // Check if the current node is an PRODUIT operator
    if (strcmp(tree->value, "PRODUIT") == 0) {
        printf("Checking semantics for PRODUIT node...\n");

        // Perform semantic comparison between left and right subtrees
        if (compare_semantic(tree->right, tree->left)) {
            printf("Semantic check passed for PRODUIT node.\n");
            return tree; // Valid semantics
        } else {
            printf("Semantic check failed for PRODUIT node.\n");
            return NULL; // Invalid semantics
        }
    }

    // If not IMPLIQUE, assume the tree is already correct
    return tree;
}
