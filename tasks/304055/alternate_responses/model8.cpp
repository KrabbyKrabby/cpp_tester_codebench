#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <stddef.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <algorithm>

class AVLTree {
public:
    struct AVLNode {
        uint32_t depth = 0;
        uint32_t cnt = 0;
        AVLNode *left = nullptr;
        AVLNode *right = nullptr;
        AVLNode *parent = nullptr;
    };

private:
    static AVLNode *root;

    static uint32_t depth(const AVLNode *node) {
        return node ? node->depth : 0;
    }

    static uint32_t cnt(const AVLNode *node) {
        return node ? node->cnt : 0;
    }

    static uint32_t max(uint32_t lhs, uint32_t rhs) {
        return lhs < rhs ? rhs : lhs;
    }

public:

    static void init(AVLNode *node) {
        node->depth = 1;
        node->cnt = 1;
        node->left = node->right = node->parent = nullptr;
    }

    static void update(AVLNode *node) {
        node->depth = 1 + max(depth(node->left), depth(node->right));
        node->cnt = 1 + cnt(node->left) + cnt(node->right);
    }

    static AVLNode *rot_left(AVLNode *node) {
        AVLNode *pivot = node->right;
        node->right = pivot->left;
        pivot->left = node;

        if (node->right) {
            node->right->parent = node;
        }

        if (node->parent) {
            if (node->parent->left == node) {
                node->parent->left = pivot;
            } else {
                node->parent->right = pivot;
            }
        }

        pivot->parent = node->parent;
        node->parent = pivot;

        update(node);
        update(pivot);

        return pivot;
    }

    static AVLNode *rot_right(AVLNode *node) {
        AVLNode *pivot = node->left;
        node->left = pivot->right;
        pivot->right = node;

        if (node->left) {
            node->left->parent = node;
        }

        if (node->parent) {
            if (node->parent->left == node) {
                node->parent->left = pivot;
            } else {
                node->parent->right = pivot;
            }
        }

        pivot->parent = node->parent;
        node->parent = pivot;

        update(node);
        update(pivot);

        return pivot;
    }

    static AVLNode *fix_left(AVLNode *node) {
        if (depth(node->left->left) >= depth(node->left->right)) {
            return rot_right(node);
        } else {
            node->left = rot_left(node->left);
            return rot_right(node);
        }
    }

    static AVLNode *fix_right(AVLNode *node) {
        if (depth(node->right->right) >= depth(node->right->left)) {
            return rot_left(node);
        } else {
            node->right = rot_right(node->right);
            return rot_left(node);
        }
    }

    static AVLNode *del(AVLNode *node) {
        if (!node) {
            return nullptr;
        }

        if (!node->left && !node->right) {
            if (node->parent) {
                if (node->parent->left == node) {
                    node->parent->left = nullptr;
                } else {
                    node->parent->right = nullptr;
                }
            } else {
                root = nullptr;
            }

            delete node;
            return nullptr;
        }

        if (!node->left) {
            if (node->parent) {
                if (node->parent->left == node) {
                    node->parent->left = node->right;
                } else {
                    node->parent->right = node->right;
                }
            } else {
                root = node->right;
            }

            node->right->parent = node->parent;
            delete node;
            return node->right;
        }

        if (!node->right) {
            if (node->parent) {
                if (node->parent->left == node) {
                    node->parent->left = node->left;
                } else {
                    node->parent->right = node->left;
                }
            } else {
                root = node->left;
            }

            node->left->parent = node->parent;
            delete node;
            return node->left;
        }

        AVLNode *replacement = node->right;
        while (replacement->left) {
            replacement = replacement->left;
        }

        node->right = del(node->right);

        node->left->parent = replacement;
        replacement->left = node->left;

        node->right->parent = replacement;
        replacement->right = node->right;

        replacement->parent = node->parent;
        if (node->parent) {
            if (node->parent->left == node) {
                node->parent->left = replacement;
            } else {
                node->parent->right = replacement;
            }
        } else {
            root = replacement;
        }

        update(replacement);
        delete node;
        return replacement;
    }

    static AVLNode *fix(AVLNode *node) {
        while (true) {
            update(node);
            uint32_t l = depth(node->left);
            uint32_t r = depth(node->right);
            AVLNode **from = NULL;
            if (node->parent) {
                from = (node->parent->left == node)
                    ? &node->parent->left : &node->parent->right;
            }
            if (l == r + 2) {
                node = fix_left(node);
            } else if (l + 2 == r) {
                node = fix_right(node);
            }
            if (!from) {
                return node;
            }
            *from = node;
            node = node->parent;
        }
    }
};

#endif //__AVL_TREE__
