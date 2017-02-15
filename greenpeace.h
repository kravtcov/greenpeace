#ifndef GREANPEACE_H
#define GREANPEACE_H

struct Node
{
    struct Node *p;
    struct Node *left;
    struct Node *right;
    int value;
};

typedef struct Node Node;
Node* lca(Node *x, Node *y);
#endif

