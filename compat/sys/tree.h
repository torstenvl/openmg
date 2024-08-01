#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdlib.h>

// Define the required entry structure for red-black tree nodes
#define RB_ENTRY(T)                  \
    struct {                         \
        struct T *left, *right, *parent;    \
        enum { RED, BLACK } color; \
    }

// Macro to define the root structure
#define RB_HEAD(T, ST)               \
    struct T {                       \
        struct ST *root;             \
    }

// Initializer for the red-black tree root
#define RB_INITIALIZER(rootptr) {}

// Define helper macros for user operations
#define RB_EMPTY(rootptr)            ((rootptr)->root == NULL)
#define RB_INSERT(T, rootptr, nodeptr) insert_##T((rootptr), (nodeptr))
#define RB_REMOVE(T, rootptr, nodeptr) remove_##T((rootptr), (nodeptr))
#define RB_FIND(T, rootptr, nodeptr)  find_##T((rootptr), (nodeptr))
#define RB_MIN(T, rootptr)           min_##T((rootptr))
#define RB_NEXT(T, rootptr, nodeptr) next_##T((nodeptr))


// Generate red-black tree functions and helpers
#define RB_GENERATE(T, ST, E, CMP)                                 \
    static void leftRotate_##T(struct T *tree, struct ST *x) {     \
        struct ST *y = x->E.right;                                 \
        x->E.right = y->E.left;                                    \
        if (y->E.left != NULL) {                                   \
            y->E.left->E.parent = x;                               \
        }                                                          \
        y->E.parent = x->E.parent;                                 \
        if (x->E.parent == NULL) {                                 \
            tree->root = y;                                        \
        } else if (x == x->E.parent->E.left) {                     \
            x->E.parent->E.left = y;                               \
        } else {                                                   \
            x->E.parent->E.right = y;                              \
        }                                                          \
        y->E.left = x;                                             \
        x->E.parent = y;                                           \
    }                                                              \
    static void rightRotate_##T(struct T *tree, struct ST *x) {    \
        struct ST *y = x->E.left;                                  \
        x->E.left = y->E.right;                                    \
        if (y->E.right != NULL) {                                  \
            y->E.right->E.parent = x;                              \
        }                                                          \
        y->E.parent = x->E.parent;                                 \
        if (x->E.parent == NULL) {                                 \
            tree->root = y;                                        \
        } else if (x == x->E.parent->E.right) {                    \
            x->E.parent->E.right = y;                              \
        } else {                                                   \
            x->E.parent->E.left = y;                               \
        }                                                          \
        y->E.right = x;                                            \
        x->E.parent = y;                                           \
    }                                                              \
    static void balanceInsert_##T(struct T *tree, struct ST *k) {  \
        while (k->E.parent != NULL && k->E.parent->E.color == RED) {\
            if (k->E.parent == k->E.parent->E.parent->E.left) {    \
                struct ST *u = k->E.parent->E.parent->E.right;     \
                if (u != NULL && u->E.color == RED) {              \
                    u->E.color = BLACK;                            \
                    k->E.parent->E.color = BLACK;                  \
                    k->E.parent->E.parent->E.color = RED;          \
                    k = k->E.parent->E.parent;                     \
                } else {                                           \
                    if (k == k->E.parent->E.right) {               \
                        k = k->E.parent;                           \
                        leftRotate_##T(tree, k);                   \
                    }                                              \
                    k->E.parent->E.color = BLACK;                  \
                    k->E.parent->E.parent->E.color = RED;          \
                    rightRotate_##T(tree, k->E.parent->E.parent);  \
                }                                                  \
            } else {                                               \
                struct ST *u = k->E.parent->E.parent->E.left;      \
                if (u != NULL && u->E.color == RED) {              \
                    u->E.color = BLACK;                            \
                    k->E.parent->E.color = BLACK;                  \
                    k->E.parent->E.parent->E.color = RED;          \
                    k = k->E.parent->E.parent;                     \
                } else {                                           \
                    if (k == k->E.parent->E.left) {                \
                        k = k->E.parent;                           \
                        rightRotate_##T(tree, k);                  \
                    }                                              \
                    k->E.parent->E.color = BLACK;                  \
                    k->E.parent->E.parent->E.color = RED;          \
                    leftRotate_##T(tree, k->E.parent->E.parent);   \
                }                                                  \
            }                                                      \
        }                                                          \
        tree->root->E.color = BLACK;                               \
    }                                                              \
    static struct ST *insert_##T(struct T *tree, struct ST *node) {\
        struct ST *y = NULL;                                       \
        struct ST *x = tree->root;                                 \
        while (x != NULL) {                                        \
            y = x;                                                 \
            int cmp = CMP(node, x);                                \
            if (cmp < 0) {                                         \
                x = x->E.left;                                     \
            } else if (cmp > 0) {                                  \
                x = x->E.right;                                    \
            } else {                                               \
                return x;                                          \
            }                                                      \
        }                                                          \
        node->E.parent = y;                                        \
        if (y == NULL) {                                           \
            tree->root = node;                                     \
        } else if (CMP(node, y) < 0) {                             \
            y->E.left = node;                                      \
        } else {                                                   \
            y->E.right = node;                                     \
        }                                                          \
        node->E.left = NULL;                                       \
        node->E.right = NULL;                                      \
        node->E.color = RED;                                       \
        balanceInsert_##T(tree, node);                             \
        return NULL;                                               \
    }                                                              \
    static void removeFixUp_##T(struct T *tree, struct ST *x) {    \
        while (x != tree->root && x->E.color == BLACK) {           \
            if (x == x->E.parent->E.left) {                        \
                struct ST *w = x->E.parent->E.right;               \
                if (w->E.color == RED) {                           \
                    w->E.color = BLACK;                            \
                    x->E.parent->E.color = RED;                    \
                    leftRotate_##T(tree, x->E.parent);             \
                    w = x->E.parent->E.right;                      \
                }                                                  \
                if (w->E.left->E.color == BLACK && w->E.right->E.color == BLACK) { \
                    w->E.color = RED;                              \
                    x = x->E.parent;                               \
                } else {                                           \
                    if (w->E.right->E.color == BLACK) {            \
                        w->E.left->E.color = BLACK;                \
                        w->E.color = RED;                          \
                        rightRotate_##T(tree, w);                  \
                        w = x->E.parent->E.right;                  \
                    }                                              \
                    w->E.color = x->E.parent->E.color;             \
                    x->E.parent->E.color = BLACK;                  \
                    w->E.right->E.color = BLACK;                   \
                    leftRotate_##T(tree, x->E.parent);             \
                    x = tree->root;                                \
                }                                                  \
            } else {                                               \
                struct ST *w = x->E.parent->E.left;                \
                if (w->E.color == RED) {                           \
                    w->E.color = BLACK;                            \
                    x->E.parent->E.color = RED;                    \
                    rightRotate_##T(tree, x->E.parent);            \
                    w = x->E.parent->E.left;                       \
                }                                                  \
                if (w->E.right->E.color == BLACK && w->E.left->E.color == BLACK) { \
                    w->E.color = RED;                              \
                    x = x->E.parent;                               \
                } else {                                           \
                    if (w->E.left->E.color == BLACK) {             \
                        w->E.right->E.color = BLACK;               \
                        w->E.color = RED;                          \
                        leftRotate_##T(tree, w);                   \
                        w = x->E.parent->E.left;                   \
                    }                                              \
                    w->E.color = x->E.parent->E.color;             \
                    x->E.parent->E.color = BLACK;                  \
                    w->E.left->E.color = BLACK;                    \
                    rightRotate_##T(tree, x->E.parent);            \
                    x = tree->root;                                \
                }                                                  \
            }                                                      \
        }                                                          \
        x->E.color = BLACK;                                        \
    }                                                              \
    static void remove_##T(struct T *tree, struct ST *z) {         \
        struct ST *x, *y, *node;                                   \
        if (!z || z == NULL) return;                               \
        if (z->E.left == NULL || z->E.right == NULL) {             \
            y = z;                                                 \
        } else {                                                   \
            y = z->E.right;                                        \
            while (y->E.left != NULL) y = y->E.left;               \
        }                                                          \
        x = (y->E.left != NULL) ? y->E.left : y->E.right;          \
        if (x != NULL) {                                           \
            x->E.parent = y->E.parent;                             \
        }                                                          \
        if (y->E.parent == NULL) {                                 \
            tree->root = x;                                        \
        } else if (y == y->E.parent->E.left) {                     \
            y->E.parent->E.left = x;                               \
        } else {                                                   \
            y->E.parent->E.right = x;                              \
        }                                                          \
        if (y != z) {                                              \
            z->E.left = y->E.left;                                 \
            z->E.right = y->E.right;                               \
            z->E.parent = y->E.parent;                             \
            z->E.color = y->E.color;                               \
            if (y->E.parent->E.left == y) {                        \
                y->E.parent->E.left = z;                           \
            } else {                                               \
                y->E.parent->E.right = z;                          \
            }                                                      \
            if (y->E.left != NULL) y->E.left->E.parent = z;        \
            if (y->E.right != NULL) y->E.right->E.parent = z;      \
        }                                                          \
        if (y->E.color == BLACK) {                                 \
            if (x) removeFixUp_##T(tree, x);                       \
        }                                                          \
    }                                                              \
    static struct ST *min_##T(const struct T *tree) {              \
        struct ST *x = tree->root;                                 \
        while (x && x->E.left != NULL) {                           \
            x = x->E.left;                                         \
        }                                                          \
        return x;                                                  \
    }                                                              \
    static struct ST *find_##T(const struct T *tree, const struct ST *node) {\
        struct ST *current = tree->root;                           \
        while (current != NULL) {                                  \
            int cmp = CMP((struct ST *)node, (struct ST *)current);\
            if (cmp < 0) {                                         \
                current = current->E.left;                         \
            } else if (cmp > 0) {                                  \
                current = current->E.right;                        \
            } else {                                               \
                return current;                                    \
            }                                                      \
        }                                                          \
        return NULL;                                               \
    }                                                              \
    static struct ST *next_##T(struct ST *node) {                  \
        if (node->E.right != NULL) {                               \
            node = node->E.right;                                  \
            while (node->E.left != NULL) {                         \
                node = node->E.left;                               \
            }                                                      \
            return node;                                           \
        }                                                          \
        struct ST *y = node->E.parent;                             \
        while (y != NULL && node == y->E.right) {                  \
            node = y;                                              \
            y = y->E.parent;                                       \
        }                                                          \
        return y;                                                  \
    }

#endif // RED_BLACK_TREE_H
