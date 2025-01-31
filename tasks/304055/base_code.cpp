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

    // Definitions outside the class to avoid clutter
    static AVLNode *rot_left(AVLNode *node) {
        // Implement rotation here
    }

    static AVLNode *rot_right(AVLNode *node) {
        // Implement rotation here
    }

    static AVLNode *fix_left(AVLNode *root) {
        // Implement fix for left-heavy imbalance
    }

    static AVLNode *fix_right(AVLNode *root) {
        // Implement fix for right-heavy imbalance
    }

    static AVLNode *del(AVLNode *node) {
        // Implement deletion here
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