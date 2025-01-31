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
        AVLNode *right = node->right;
        AVLNode *right_left = right->left;
        AVLNode *parent = node->parent;

        right->left = node;
        node->right = right_left;
        right->parent = parent;
        node->parent = right;
        
        if (right_left) {
            right_left->parent = node;
        }

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
        left->parent = parent;
        node->parent = left;
        
        if (left_right) {
            left_right->parent = node;
        }

        update(node);
        update(left);
        return left;
    }

    static AVLNode *fix_left(AVLNode *root) {
        if (depth(root->left->left) < depth(root->left->right)) {
            root->left = rot_left(root->left);
        }
        return rot_right(root);
    }

    static AVLNode *fix_right(AVLNode *root) {
        if (depth(root->right->right) < depth(root->right->left)) {
            root->right = rot_right(root->right);
        }
        return rot_left(root);
    }

    static AVLNode *del(AVLNode *node) {
        AVLNode *parent = node->parent;
        
        // Case 1: Node has no children or only one child
        if (!node->left || !node->right) {
            AVLNode *child = node->left ? node->left : node->right;
            if (parent) {
                if (parent->left == node) {
                    parent->left = child;
                } else {
                    parent->right = child;
                }
            }
            if (child) {
                child->parent = parent;
            }
            return parent ? fix(parent) : child;
        }
        
        // Case 2: Node has two children
        // Find successor (minimum element in right subtree)
        AVLNode *succ = node->right;
        while (succ->left) {
            succ = succ->left;
        }
        
        // Remove successor from its current position
        AVLNode *new_root = del(succ);
        
        // Put successor in place of node
        succ->left = node->left;
        succ->right = node->right;
        succ->parent = node->parent;
        
        if (node->left) {
            node->left->parent = succ;
        }
        if (node->right) {
            node->right->parent = succ;
        }
        if (parent) {
            if (parent->left == node) {
                parent->left = succ;
            } else {
                parent->right = succ;
            }
        }
        
        return fix(succ);
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