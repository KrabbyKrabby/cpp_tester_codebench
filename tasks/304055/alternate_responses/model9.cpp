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

    // Rotation operations
    static AVLNode *rot_left(AVLNode *node) {
        AVLNode *right = node->right;
        AVLNode *right_left = right->left;
        AVLNode *parent = node->parent;

        right->left = node;
        node->right = right_left;
        node->parent = right;

        if (right_left) {
            right_left->parent = node;
        }

        if (parent) {
            if (parent->left == node) {
                parent->left = right;
            } else {
                parent->right = right;
            }
        } else {
            root = right;
        }

        right->parent = parent;
        update(node);
        update(right);

        return right;
    }

    static AVLNode *rot_right(AVLNode *node) {
        AVLNode *left = node->left;
        AVLNode *left_right = left->right;
        AVLNode *parent = node->parent;

        left->right = node;
        node->left = left_right;
        node->parent = left;

        if (left_right) {
            left_right->parent = node;
        }

        if (parent) {
            if (parent->left == node) {
                parent->left = left;
            } else {
                parent->right = left;
            }
        } else {
            root = left;
        }

        left->parent = parent;
        update(node);
        update(left);

        return left;
    }

    // Balancing operations
    static AVLNode *fix_left(AVLNode *node) {
        AVLNode *left = node->left;
        if (depth(left->left) >= depth(left->right)) {
            return rot_right(node);
        } else {
            left = rot_left(left);
            node->left = left;
            return rot_right(node);
        }
    }

    static AVLNode *fix_right(AVLNode *node) {
        AVLNode *right = node->right;
        if (depth(right->right) >= depth(right->left)) {
            return rot_left(node);
        } else {
            right = rot_right(right);
            node->right = right;
            return rot_left(node);
        }
    }

    // Deletion operation
    static AVLNode *del(AVLNode *node) {
        AVLNode *left = node->left;
        AVLNode *right = node->right;
        AVLNode *parent = node->parent;

        if (!left && !right) {
            if (parent) {
                if (parent->left == node) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            } else {
                root = nullptr;
            }
        } else if (!left) {
            if (parent) {
                if (parent->left == node) {
                    parent->left = right;
                } else {
                    parent->right = right;
                }
            } else {
                root = right;
            }
            right->parent = parent;
        } else if (!right) {
            if (parent) {
                if (parent->left == node) {
                    parent->left = left;
                } else {
                    parent->right = left;
                }
            } else {
                root = left;
            }
            left->parent = parent;
        } else {
            AVLNode *replacement = right;
            while (replacement->left) {
                replacement = replacement->left;
            }
            node->right = del(replacement);
            node->left = left;
            node->parent = parent;
            left->parent = node;
            if (parent) {
                if (parent->left == replacement) {
                    parent->left = node;
                } else {
                    parent->right = node;
                }
            } else {
                root = node;
            }
        }

        return parent ? fix(parent) : nullptr;
    }

    // Fixing imbalance
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
