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
        AVLNode *rhs = node->right;
        AVLNode *temp = rhs->left;
        rhs->left = node;
        node->right = temp;
        node->parent = rhs;
        if (temp) {
            temp->parent = node;
        }
        update(node);
        update(rhs);
        return rhs;
    }

    // Implement rotation right
    static AVLNode *rot_right(AVLNode *node) {
        AVLNode *lhs = node->left;
        AVLNode *temp = lhs->right;
        lhs->right = node;
        node->left = temp;
        node->parent = lhs;
        if (temp) {
            temp->parent = node;
        }
        update(node);
        update(lhs);
        return lhs;
    }

    // Implement fix for left-heavy imbalance
    static AVLNode *fix_left(AVLNode *node) {
        if (depth(node->left->left) >= depth(node->left->right)) {
            return rot_right(node);
        } else {
            node->left = rot_left(node->left);
            return rot_right(node);
        }
    }

    // Implement fix for right-heavy imbalance
    static AVLNode *fix_right(AVLNode *node) {
        if (depth(node->right->right) >= depth(node->right->left)) {
            return rot_left(node);
        } else {
            node->right = rot_right(node->right);
            return rot_left(node);
        }
    }

    // Implement deletion
    static AVLNode *del(AVLNode *node) {
        if (!node->left) {
            AVLNode *temp = node->right;
            if (temp) {
                temp->parent = node->parent;
            }
            return temp;
        } else if (!node->right) {
            AVLNode *temp = node->left;
            if (temp) {
                temp->parent = node->parent;
            }
            return temp;
        } else {
            AVLNode *temp = node->right;
            while (temp->left) {
                temp = temp->left;
            }
            node->right = del(node->right);
            node->right->parent = node;
            temp->left = node->left;
            temp->right = node->right;
            temp->parent = node->parent;
            node->left->parent = temp;
            node->right->parent = temp;
            return fix(temp);
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
