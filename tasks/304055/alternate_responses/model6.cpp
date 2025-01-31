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

    // Implement rotation left
    static AVLNode *rot_left(AVLNode *node) {
        AVLNode *right = node->right;
        AVLNode *right_left = right->left;

        right->left = node;
        node->right = right_left;

        node->parent = right;
        if (right_left) {
            right_left->parent = node;
        }

        update(node);
        update(right);

        return right;
    }

    // Implement rotation right
    static AVLNode *rot_right(AVLNode *node) {
        AVLNode *left = node->left;
        AVLNode *left_right = left->right;

        left->right = node;
        node->left = left_right;

        node->parent = left;
        if (left_right) {
            left_right->parent = node;
        }

        update(node);
        update(left);

        return left;
    }

    // Implement fix for left-heavy imbalance
    static AVLNode *fix_left(AVLNode *root) {
        if (depth(root->left->left) >= depth(root->left->right)) {
            return rot_right(root);
        } else {
            root->left = rot_left(root->left);
            return rot_right(root);
        }
    }

    // Implement fix for right-heavy imbalance
    static AVLNode *fix_right(AVLNode *root) {
        if (depth(root->right->right) >= depth(root->right->left)) {
            return rot_left(root);
        } else {
            root->right = rot_right(root->right);
            return rot_left(root);
        }
    }

    // Implement deletion
    static AVLNode *del(AVLNode *node) {
        if (!node->left) {
            AVLNode *right = node->right;
            if (right) {
                right->parent = node->parent;
            }
            return right;
        } else if (!node->right) {
            AVLNode *left = node->left;
            if (left) {
                left->parent = node->parent;
            }
            return left;
        } else {
            AVLNode *replace = node->right;
            while (replace->left) {
                replace = replace->left;
            }
            node->right = del(replace);
            node->right->parent = node;
            replace->left = node->left;
            replace->left->parent = replace;
            replace->right = node->right;
            replace->right->parent = replace;
            replace->parent = node->parent;
            return replace;
        }
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