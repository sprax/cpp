// AVL Tree without parent pointers.  Not working, yet.
// S.R. Lines 2016.07

#include "stdafx.h"

/** basic int-value binary-tree node */
typedef struct node
{
int val;
struct node* left;
struct node* right;
int ht;
} node;

node * newNode(int val) {
    node *root = new node();
    root->val = val;
    root->left = NULL;
    root->right = NULL;
    root->ht = 1;
    return root;
}

int height(node * nd) {
    return nd == NULL ? 0 : nd->ht;
}

int balance(node * cur) {
    return height(cur->left) - height(cur->right);
}

void swapVals(node *one, node *two) {
    int tmp = one->val;
    one->val = two->val;
    two->val = tmp;
}

// Assuming val is truly not already in the tree, so we don't traverse just to check that.
node * insert(node * root, int val)
{
    // special case:
    if (root == NULL) {
        return root = newNode(val);
    }
    node * par = NULL;
    node * cur = root;
    int bal = 0;            // balance at the parent of the current node
    bool must_retrace = false;
    while (cur != NULL) {
        if (val < cur->val) {
            if (cur->left == NULL) {
                cur->left = newNode(val);
                if (cur->right == NULL) {
                    if (par != NULL && par->right == NULL) {             // parent had NULL right
                        // Left-Left Case
                        swapVals(par, cur);
                        par->right = cur;
                        par->left = cur->left;
                        cur->left = NULL;
                        //par->ht = 2;
                        //par->left->ht = par->right->ht = 1; // already true
                    }
                    else if (par != NULL && par->left == NULL) {       // parent had NULL left
                        // Right Left Case
                        swapVals(par, cur->left);
                        par->left = cur->left;
                        cur->left = NULL;
                        //par->ht = 2;
                    }
                    else {
                        cur->ht++;
                        must_retrace = true;
                    }
                }
                break;
            }
            else {
                bal = balance(cur);
                par = cur;
                cur = cur->left;
            }
        }
        else if (val > cur->val) {
            if (cur->right == NULL) {
                cur->right = newNode(val);
                if (cur->left == NULL) {
                    if (par != NULL && par->right == NULL) {         // parent had no right
                        // Left Right Case
                        swapVals(par, cur->right);
                        par->right = cur->right;
                        cur->right = NULL;
                        //par->ht = 2;
                    }
                    else if (par != NULL && par->left == NULL) {   // parent had no left
                        // Right Right Case
                        swapVals(par, cur);
                        par->left = cur;
                        par->right = cur->right;
                        cur->right = NULL;
                        //par->ht = 2;
                    }
                    else {
                        cur->ht++;
                        must_retrace = true;
                    }
                }
                break;
            }
            else {
                bal = balance(cur);
                par = cur;
                cur = cur->right;
            }
        }
        else {
            break;  // val is already in the tree, so do nothing
        }
    }
    if (must_retrace) {
        par = root;
        while (par != cur) {
            bal = balance(par);
            if (val < par->val) {
                if (bal > 1) {
                    printf("balance 2 at %d\n", par->val);
                    par->ht++;
                }
                par = par->left;
            }
            else {
                if (bal < -1) {
                    printf("balance -2 at %d\n", par->val);
                    par->ht++;
                }
                par = par->right;
            }
        }
    }
    return root;
}

void printInOrder(node * root)
{
    if (root) {
        printInOrder(root->left);
        printf("(%d  %d) ", root->val, root->ht);
        printInOrder(root->right);
    }
}

int test_AVL_Tree_insert()
{
    node * tree;

    // Left Left
    tree = NULL;
    tree = insert(tree, 5);
    tree = insert(tree, 4);
    tree = insert(tree, 3);
    tree = insert(tree, 2);
    printInOrder(tree);
    puts("");
    tree = insert(tree, 6);
    printInOrder(tree);
    puts("");

    // Left Right
    tree = NULL;
    tree = insert(tree, 5);
    tree = insert(tree, 3);
    tree = insert(tree, 4);
    printInOrder(tree);
    puts("");

    // Right Left
    tree = NULL;
    tree = insert(tree, 3);
    tree = insert(tree, 5);
    tree = insert(tree, 4);
    printInOrder(tree);
    puts("");

    // Right Right
    tree = NULL;
    tree = insert(tree, 3);
    tree = insert(tree, 4);
    tree = insert(tree, 5);
    tree = insert(tree, 6);
    tree = insert(tree, 7);
    printInOrder(tree);
    puts("");

    return 0;
}