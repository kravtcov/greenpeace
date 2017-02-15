#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#include "greenpeace.h"

typedef struct
{
    Node *x_left;
    Node *x_right;
    Node *y_left;
    Node *y_right;
} Children;

void deleteChildren(Node *x, Node *y)
{
    x->left = NULL;
    x->right = NULL;
    y->left = NULL;
    y->right = NULL;
}

Children saveChildren(Node *x, Node *y)
{
    Children ch;

    ch.x_left = x->left;
    ch.x_right = x->right;
    ch.y_left = y->left;
    ch.y_right = y->right;

    return ch;
}

void restoreChildren(Node *x, Node *y, Children ch)
{
    x->left = ch.x_left;
    x->right = ch.x_right;
    y->left = ch.y_left;
    y->right = ch.y_right;
}

bool isParentBrokenForBorken(Node *x_broken, Node *x_child_with_no_true_par)
{
    if (x_broken == NULL)
        return false;

    Node *true_par = (x_broken->left == x_child_with_no_true_par)
                     ? x_broken->right : x_broken->left;
    if (true_par == NULL)
        return false;
    return (true_par->p == x_broken);
}

bool isNotRelatives(Node *x, Node *y)
{
    if ((x == NULL) || (y == NULL))
        return false;

    return !(x->p == y) && !(y->p == x);
}

bool parentsNotBrokenForBrokens(Node *x_broken, Node *x_child_with_no_true_par,
                               Node *y_broken, Node *y_child_with_no_true_par)
{
    return (   !isParentBrokenForBorken(x_broken, x_child_with_no_true_par)
            && !isParentBrokenForBorken(y_broken, y_child_with_no_true_par));
}

bool isParentBroken(Node *x)
{
    if ((x == NULL) || (x->p == NULL))
        return false;
    //return !(x->p->left == x) && !(x->p->right == x);
    return (x->p->p == x);
}

bool parentsNotBroken(Node *x, Node *y)
{
    return !isParentBroken(x) && !isParentBroken(y);
}

bool oneStillBroken(Node *x, Node *y)
{
    return (isParentBroken(x) || isParentBroken(y));
}

void swap(Node *x, Node *y)
{
    Node *tmp = x;
    x = y;
    y = tmp;
}

void turn(Node *x, Node *fixed_child)
{
    if (x == NULL)
        return;
    
    swap(x->p, (x->left == fixed_child) ? x->right : x->left);
}


void repairParents(Node *x, Node *y)
{
    Node *i_prev = x;
    Node *j_prev = y;
    Node *i = x->p;
    Node *j = y->p;

    while (oneStillBroken(i, j)) {
        turn(i, i_prev);
        turn(j, j_prev);

        i_prev = i;
        j_prev = j;
        i = (i != NULL) ? i->p : NULL;
        j = (j != NULL) ? j->p : NULL;
    }
}

Node* destroyParents(Node *x, Node *y)
{
    Children ch = saveChildren(x, y);
    deleteChildren(x, y);

    Node *i_prev = x;
    Node *j_prev = y;
    Node *i = x->p;
    Node *j = y->p;

    while ((i != j) && parentsNotBroken(i, j) && isNotRelatives(i, j)) {
        Node *parent_i = (i != NULL) ? i->p : NULL;
        Node *parent_j = (j != NULL) ? j->p : NULL;
        
        turn(i, i_prev);
        turn(j, j_prev);

        //if (!parentsNotBrokenForBrokens(i, i_prev, j, j_prev))
            i_prev = i;
            j_prev = j;
            i = parent_i;
            j = parent_j;
       // }
    }

    restoreChildren(x, y, ch);

    // without saving children
    // if ((i == x) || (j == x))
    //     return x;
    // if ((i == y) || (j == y))
    //     return y;
    return (i == j) ? i
                    : isParentBroken(i) ? i->p
                                        : j->p;
}

Node* lca(Node *x, Node *y)
{
    if (x == y)
        return x;
    if ((x == NULL) || (y == NULL))
        return NULL;
    
    Node *lca = destroyParents(x, y);
    repairParents(x, y);
    return lca;
}

