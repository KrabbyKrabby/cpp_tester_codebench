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
        AVLNode *parent = node->parent;
        
        node->right = right->left;
        if (right->left) {
            right->left->parent = node;
        }
        
        right->left = node;
        right->parent = parent;
        node->parent = right;
        
        update(node);
        update(right);
        return right;
    }

    static AVLNode *rot_right(AVLNode *node) {
        AVLNode *left = node->left;
        AVLNode *parent = node->parent;
        
        node->left = left->right;
        if (left->right) {
            left->right->parent = node;
        }
        
        left->right = node;
        left->parent = parent;
        node->parent = left;
        
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
        if (!node) return nullptr;

        // Case 1: Node is a leaf
        if (!node->left && !node->right) {
            if (node->parent) {
                if (node->parent->left == node)
                    node->parent->left = nullptr;
                else
                    node->parent->right = nullptr;
                
                AVLNode *parent = node->parent;
                delete node;
                return fix(parent);
            }
            delete node;
            return nullptr;
        }

        // Case 2: Node has only one child
        if (!node->left || !node->right) {
            AVLNode *child = node->left ? node->left : node->right;
            child->parent = node->parent;
            
            if (node->parent) {
                if (node->parent->left == node)
                    node->parent->left = child;
                else
                    node->parent->right = child;
                
                AVLNode *parent = node->parent;
                delete node;
                return fix(parent);
            }
            
            delete node;
            return child;
        }

        // Case 3: Node has both children
        // Find successor (minimum element in right subtree)
        AVLNode *succ = node->right;
        while (succ->left) {
            succ = succ->left;
        }

        // Copy successor's data to current node
        // Note: In a real implementation, you'd copy all relevant data fields
        
        // Delete the successor
        AVLNode *new_root = del(succ);
        if (node->parent == nullptr) {
            root = new_root;
        }
        
        return new_root;
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