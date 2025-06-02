// File: btree.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"

// Function for creating a node with a describing whether it is an input node or output node
BTreeNode *create_node(int a)
{
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->is_leaf = a;
    node->num_keys = 0;
    for (int i = 0; i < ORDER; i++)
        node->children[i] = NULL;
    return node;
}

// Allocating memory for Btree and creating its root(at this time root is a leaf node only!!)
BTree *create_btree()
{
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    tree->root = create_node(1);
    return tree;
}

// Function for searching the tree on the basis of the primary key
char *search(BTree *tree, int key)
{
    BTreeNode *node = tree->root;
    while (node != NULL)
    {
        int i = 0;
        while (i < node->num_keys && key > node->keys[i])
            i++;
        if (i < node->num_keys && key == node->keys[i])
            return node->values[i];
        if (node->is_leaf)
            return NULL;
        node = node->children[i];
    }
    return NULL;
}

// Function for level order traversal of the tree which first prints all the nodes at a given level
// and then pushes the children at the next level into the queue

void level_order_traversal(BTree *tree)
{
    if (!tree || !tree->root)
        return;

    BTreeNode *queue[100];
    int front = 0, rear = 0;
    queue[rear++] = tree->root;

    while (front < rear)
    {
        int size = rear - front;

        for (int i = 0; i < size; i++)
        { // Printing Nodes at a given level
            BTreeNode *node = queue[front++];

            // Printing key-values in a given node
            for (int j = 0; j < node->num_keys; j++)
            {
                printf("[%d,%s] ", node->keys[j], node->values[j]);
            }

            // Pushing the children at the next level into the queue if the current node is not a leaf
            if (!node->is_leaf)
            {
                for (int j = 0; j <= node->num_keys; j++)
                    queue[rear++] = node->children[j];
            }
        }
    }
}

// Function to split children if new values are required to be inserted or previous values have to be
// deleted. Explained in more detail below.

void split_child(BTreeNode *parent, int i)
{ // ith node is already full of children

    BTreeNode *child = parent->children[i];
    BTreeNode *new_node = create_node(child->is_leaf);

    int t = (ORDER - 1) / 2;
    int right_count = child->num_keys - t - 1;

    // We split the keys into 3 parts - 2 equal parts and the middle value is pushed up

    for (int j = 0; j < right_count; j++)
    {
        int src_idx = j + t + 1;
        new_node->keys[j] = child->keys[src_idx];
        strncpy(new_node->values[j],child->values[src_idx],MAX_LEN);
    }
    new_node->num_keys = right_count;

    // 2) If child is not leaf, copy the corresponding child pointers
    if (!child->is_leaf)
    {
        for (int j = 0; j <= right_count; j++)
        {
            new_node->children[j] = child->children[j + t + 1];
        }
    }

    // 3) Reduce child’s key count to t
    child->num_keys = t;

    // 4) Shift parent’s children pointers right from index (i+1) onward
    for (int j = parent->num_keys; j >= i + 1; j--)
    {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = new_node;

    // Making shifts so that we can allocate a pointer for the new node into the original parent
    for (int j = parent->num_keys - 1; j >= i; j--)
    {
        parent->keys[j + 1] = parent->keys[j];
        strncpy(parent->values[j + 1], parent->values[j],MAX_LEN);
    }

    // 6) Promote child’s median key and value into parent at index i
    parent->keys[i] = child->keys[t];
    strncpy(parent->values[i], child->values[t], MAX_LEN);

    parent->num_keys++;
}

// Function to insert a key-value pair into the tree in a node which has not reached its maximum
// capacity of carrying the key-value pairs(here 4). If the maximum capacity is reached then the node is
// first split into 2 using the above split function and then the new value is inserted

void insert_nnode(BTreeNode *node, int key, const char *value)
{
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        // If the node is a lead node we shift all the keys greater than the given key one place to the right
        // and then insert the given key at the (i+1)th index
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            strncpy(node->values[i + 1], node->values[i], MAX_LEN);
            i--;
        }
        // Insert the key at the index and also copy its value into the given value
        node->keys[i + 1] = key;
        strncpy(node->values[i + 1], value, MAX_LEN);
        node->num_keys++;
    } else {
        // If it is not a leaf node then find the appropriate index first
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++; 
        
         // If the given child node is already full then first split it and then insert the key-value pair into the node
        if (node->children[i]->num_keys == ORDER - 1) {
            split_child(node, i);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insert_nnode(node->children[i], key, value);
    }
}

void insert(BTree *tree, int key, const char *value)
{
    BTreeNode *r = tree->root;

    // If the root node is already full then we first create new root split it and then call the above function otherwise
    // we straight away call the above function
    if (r->num_keys == ORDER - 1)
    {
        BTreeNode *s = create_node(0);
        tree->root = s;
        s->children[0] = r;
        split_child(s, 0);
        insert_nnode(s, key, value);
    }
    else
    {
        insert_nnode(r, key, value);
    }
}

// Function to free the memory for a node

void free_node(BTreeNode *node)
{
    if (!node)
        return;

    // If it is not a leaf node then recursively call free_node for all the children of the given node
    if (!node->is_leaf)
    {
        for (int i = 0; i <= node->num_keys; i++)
            free_node(node->children[i]);
    }

    free(node);
}

// Frees the memory for the entrire btree first by removing the nodes and then the tree itself is removed
void free_btree(BTree *tree)
{
    if (!tree)
        return;
    free_node(tree->root);
    free(tree);
}