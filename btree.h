// File: btree.h
#ifndef BTREE_H
#define BTREE_H

#define ORDER 5
#define MAX_LEN 100

typedef struct BTreeNode {
    int is_leaf;
    int num_keys;
    int keys[ORDER-1];
    char values[ORDER-1][MAX_LEN];
    struct BTreeNode *children[ORDER];
} BTreeNode;

typedef struct {
    BTreeNode *root;
} BTree;

BTree* create_btree();
char* search(BTree *tree, int key);
void level_order_traversal(BTree *tree);
void insert(BTree *tree, int key, const char *value);
void free_btree(BTree *tree);

#endif
