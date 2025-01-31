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
        AVLNode *tmp = node->right;
        node->right = tmp->left;
        if (tmp->left) {
            tmp->left->parent = node;
        }
        tmp->left = node;
        tmp->parent = node->parent;
        node->parent = tmp;
        update(node);
        update(tmp);
        return tmp;
    }

    static AVLNode *rot_right(AVLNode *node) {
        AVLNode *tmp = node->left;
        node->left = tmp->right;
        if (tmp->right) {
            tmp->right->parent = node;
        }
        tmp->right = node;
        tmp->parent = node->parent;
        node->parent = tmp;
        update(node);
        update(tmp);
        return tmp;
    }

    static AVLNode *fix_left(AVLNode *root) {
        if (depth(root->left->left) >= depth(root->left->right)) {
            return rot_right(root);
        } else {
            root->left = rot_left(root->left);
            return rot_right(root);
        }
    }

    static AVLNode *fix_right(AVLNode *root) {
        if (depth(root->right->right) >= depth(root->right->left)) {
            return rot_left(root);
        } else {
            root->right = rot_right(root->right);
            return rot_left(root);
        }
    }

    static AVLNode *del(AVLNode *node) {
        AVLNode *p = nullptr;
        if (!node->left) {
            p = node->right;
        } else if (!node->right) {
            p = node->left;
        } else {
            AVLNode *tmp = node->left;
            while (tmp->right) {
                tmp = tmp->right;
            }
            node->left = del(node->left);
            node->left = tmp->left;
            node->right = tmp->right;
            node->parent = tmp->parent;
            node->depth = tmp->depth;
            node->cnt = tmp->cnt;
            return fix(node);
        }
        if (p) {
            p->parent = node->parent;
        }
        return p;
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