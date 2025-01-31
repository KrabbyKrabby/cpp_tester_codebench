#include <gtest/gtest.h>
#include "solution.cpp"  // Update this include path to wherever your AVLTree definition is located


class AVLTreeTest : public ::testing::Test {
protected:
    AVLTree::AVLNode* createNode() {
        auto* node = new AVLTree::AVLNode;
        AVLTree::init(node);
        return node;
    }

    void connectNodes(AVLTree::AVLNode* parent, AVLTree::AVLNode* left, AVLTree::AVLNode* right) {
        parent->left = left;
        parent->right = right;
        if (left) left->parent = parent;
        if (right) right->parent = parent;
        AVLTree::update(parent);
    }
};

TEST_F(AVLTreeTest, RotLeftSimple) {
    auto* root = createNode();
    auto* rightChild = createNode();
    connectNodes(root, nullptr, rightChild);

    auto* newRoot = AVLTree::rot_left(root);
    ASSERT_EQ(newRoot, rightChild);
    ASSERT_EQ(newRoot->left, root);
    ASSERT_EQ(root->parent, newRoot);
    ASSERT_EQ(root->right, nullptr);
}

TEST_F(AVLTreeTest, RotRightSimple) {
    auto* root = createNode();
    auto* leftChild = createNode();
    connectNodes(root, leftChild, nullptr);

    auto* newRoot = AVLTree::rot_right(root);
    ASSERT_EQ(newRoot, leftChild);
    ASSERT_EQ(newRoot->right, root);
    ASSERT_EQ(root->parent, newRoot);
    ASSERT_EQ(root->left, nullptr);
}


TEST_F(AVLTreeTest, FixLeftWithLeftRightCase) {
    auto* root = createNode();
    auto* left = createNode();
    auto* child = createNode(); // right child of left
    connectNodes(root, left, nullptr);
    connectNodes(left, nullptr, child);

    auto* newRoot = AVLTree::fix_left(root);
    ASSERT_EQ(newRoot, child);
    ASSERT_EQ(newRoot->left, left);
    ASSERT_EQ(newRoot->right, root);
    ASSERT_EQ(root->parent, child);
    ASSERT_EQ(left->parent, child);
}

TEST_F(AVLTreeTest, FixRightWithRightLeftCase) {
    auto* root = createNode();
    auto* right = createNode();
    auto* child = createNode(); // left child of right
    connectNodes(root, nullptr, right);
    connectNodes(right, child, nullptr);

    auto* newRoot = AVLTree::fix_right(root);
    ASSERT_EQ(newRoot, child);
    ASSERT_EQ(newRoot->right, right);
    ASSERT_EQ(newRoot->left, root);
    ASSERT_EQ(root->parent, child);
    ASSERT_EQ(right->parent, child);
}


// del handles the removal of a node and needs a complex scenario for thorough testing:
TEST_F(AVLTreeTest, DeleteNodeWithTwoChildren) {
    auto* root = createNode();
    auto* left = createNode();
    auto* right = createNode();
    connectNodes(root, left, right);
    auto* newRoot = AVLTree::del(root);

    // Check new root and structure after deletion
    ASSERT_TRUE(newRoot != root); // Assuming it picks one of the children
    ASSERT_EQ(newRoot->parent, nullptr);
    ASSERT_TRUE(newRoot->left == left || newRoot->right == right);
}