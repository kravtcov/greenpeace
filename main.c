#include "greenpeace.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_DEPTH 5
#define TREE_SIZE 31

Node *createNode(int value) 
{
    Node *x = (Node *)malloc(sizeof(Node));
    x->value = value;
    return x;
}

Node* createSubTree(Node *p, int depth)
{
    if (depth == MAX_DEPTH)
        return NULL;
    Node *x = createNode(depth);
    x->p = p;
    x->left  = createSubTree(x, depth + 1);
    x->right = createSubTree(x, depth + 1);
    return x;
}

Node* createTree()
{
    return createSubTree(NULL, 0);
}

void deleteTree(Node *x)
{
    if (x->right != NULL)
        deleteTree(x->right);
    if (x->left != NULL)
        deleteTree(x->left);
    free(x);
}

void printNode(Node *x)
{
    for (int i = 0; i < x->value * 8; i++) {
        printf(" ");
    }
    printf("%#08x\n", (int)x);
}

void printTree(Node *x)
{
    if (x == NULL)
        return;
    printTree(x->left);
    printNode(x);
    printTree(x->right);
}

void treeAsArray(Node* x, Node **array, int *i)
{
    if (x == NULL)
        return;
    array[(*i)++] = x;
    treeAsArray(x->left, array, i);
    treeAsArray(x->right, array, i);
}

void printArray(Node** array, int size)
{
    printf("Array:\n\n");
    for (int i = 0; i < size; i++)
    {
        printf("%#08x ", (int)array[i]);
    }
    printf("\n\n");
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        perror("ERR: args number");
        return -1;
    }

    int first = atoi(argv[1]);
    int second = atoi(argv[2]);

    Node *r = createTree();
    Node **array = (Node **)malloc(TREE_SIZE * sizeof(Node * ));

    int i = 0;
    treeAsArray(r, array, &i);

    printArray(array, TREE_SIZE);
    printTree(r);

//  printf("%#08x ", (int)(array[first]));
//  printf("%#08x ", (int)(array[first]->p));
//  printf("%#08x ", (int)(array[first]->p->p));
//  printf("%#08x ", (int)(array[second]));
    Node *l_c_a = lca(array[first], array[second]);
    printf("f %#08x\n", (int)(array[first]));
    printf("s %#08x\n", (int)(array[second]));
    printf("l %#08x\n", (int)l_c_a);
    printTree(r);

    deleteTree(r);
    free(array);
    return 0;
}

