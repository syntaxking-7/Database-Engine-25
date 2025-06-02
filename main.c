// File: main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"

#define MAX_VALUE_LEN 100
void print_menu() {
    printf("\nChoose an operation:\n");
    printf("1 - Insert key and value\n");
    printf("2 - Search key\n");
    printf("3 - Print B-Tree (level order)\n");
    printf("4 - Exit\n");
    printf("Enter choice: ");
}

int main() {

    BTree *tree = create_btree();

    insert(tree, 10, "Ten");
    insert(tree, 20, "Twenty");
    insert(tree, 5, "Five");
    insert(tree, 6, "Six");
    insert(tree, 12, "Twelve");
    insert(tree, 30, "Thirty");
    insert(tree, 7, "Seven");
    insert(tree, 17, "Seventeen");

    printf("Initial tree created with some keys.\n");
    level_order_traversal(tree);

    int choice, key;
    char value[MAX_VALUE_LEN];

    while (1) {
        print_menu();
        if (scanf("%d",&choice)!=1) {
            printf("Invalid input. Exiting.\n");
            break;
        }
        getchar(); 

        switch (choice) {
            case 1:
                printf("Enter key : ");
                if (scanf("%d",&key) != 1) {
                    printf("Invalid key input.\n");
                    getchar(); 
                    break;
                }
                getchar(); 
                printf("Enter value (string): ");
                if (fgets(value, sizeof(value), stdin) == NULL) {
                    printf("Invalid value input.\n");
                    break;
                }
                value[strcspn(value, "\n")] = 0; 
                insert(tree, key, value);
                printf("Inserted key %d with value \"%s\"\n", key, value);
                break;

            case 2:
                printf("Enter key to search: ");
                if (scanf("%d", &key) != 1) {
                    printf("Invalid key input.\n");
                    getchar();
                    break;
                }
                getchar();
                {
                    char *res = search(tree, key);
                    if (res) printf("Key %d found with value: %s\n", key, res);
                    else printf("Key %d not found\n", key);
                }
                break;

            case 3:
                printf("Level-order traversal:\n");
                level_order_traversal(tree);
                break;

            case 4:
                free_btree(tree);
                printf("Exiting.\n");
                return 0;

            default:
                printf("Invalid choice, please try again.\n");
                break;
        }
    }

    free_btree(tree);
    return 0;
}
