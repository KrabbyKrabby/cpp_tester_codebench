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
        AVLNode *new_node = node->right;
        if (new_node->left) {
            new_node->left->parent = node;
        }
        node->right = new_node->left;
        new_node->left = node;
        new_node->parent = node->parent;
        node->parent = new_node;
        update(node);
        update(new_node);
        return new_node;
    }

    static AVLNode *rot_right(AVLNode *node) {
        AVLNode *new_node = node->left;
        if (new_node->right) {
            new_node->right->parent = node;
        }
        node->left = new_node->right;
        new_node->right = node;
        new_node->parent = node->parent;
        node->parent = new_node;
        update(node);
        update(new_node);
        return new_node;
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

    static AVLNode *del(AVLNode *node) {
        if (node->right == NULL) {
            AVLNode *parent = node->parent;
            if (node->left) {
                node->left->parent = parent;
            }
            if (parent) {
                (parent->left == node ? parent->left : parent->right) = node->left;
                return fix(parent);
            } else {
                return node->left;
            }
        } else {
            AVLNode *victim = node->right;
            while (victim->left) {
                victim = victim->left;
            }
            AVLNode *root = del(victim);

            *victim = *node;
            if (victim->left) {
                victim->left->parent = victim;
            }
            if (victim->right) {
                victim->right->parent = victim;
            }
            AVLNode *parent = node->parent;
            if (parent) {
                (parent->left == node ? parent->left : parent->right) = victim;
                return root;
            } else {
                return victim;
            }
        }
    }
};

#endif //__AVL_TREE__