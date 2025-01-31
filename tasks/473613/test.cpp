#include <gtest/gtest.h>
#include "solution.cpp" 

// Helper function to build a test tree for test cases
TreeNode* buildTestTree() {
    TreeNode* root = new TreeNode("Ancestor");

    TreeNode* mammal = new TreeNode("Mammal");
    TreeNode* reptile = new TreeNode("Reptile");
    TreeNode* bird = new TreeNode("Bird");

    TreeNode* human = new TreeNode("Human");
    TreeNode* chimpanzee = new TreeNode("Chimpanzee");
    TreeNode* whale = new TreeNode("Whale");

    TreeNode* lizard = new TreeNode("Lizard");
    TreeNode* snake = new TreeNode("Snake");
    TreeNode* turtle = new TreeNode("Turtle");

    TreeNode* sparrow = new TreeNode("Sparrow");
    TreeNode* eagle = new TreeNode("Eagle");
    TreeNode* penguin = new TreeNode("Penguin");

    root->children.push_back(mammal);
    root->children.push_back(reptile);
    root->children.push_back(bird);

    mammal->children.push_back(human);
    mammal->children.push_back(chimpanzee);
    mammal->children.push_back(whale);

    reptile->children.push_back(lizard);
    reptile->children.push_back(snake);
    reptile->children.push_back(turtle);

    bird->children.push_back(sparrow);
    bird->children.push_back(eagle);
    bird->children.push_back(penguin);

    return root;
}

// Test cases
TEST(FindNearestCommonAncestorTest, CommonAncestorWithinSameBranch) {
    TreeNode* root = buildTestTree();
    TreeNode* result = findNearestCommonAncestor(root, "Human", "Chimpanzee");
    EXPECT_EQ(result->name, "Mammal");
}

TEST(FindNearestCommonAncestorTest, CommonAncestorAcrossDifferentBranches) {
    TreeNode* root = buildTestTree();
    TreeNode* result = findNearestCommonAncestor(root, "Human", "Lizard");
    EXPECT_EQ(result->name, "Ancestor");
}

TEST(FindNearestCommonAncestorTest, DirectDescendantCase) {
    TreeNode* root = buildTestTree();
    TreeNode* result = findNearestCommonAncestor(root, "Mammal", "Human");
    EXPECT_EQ(result->name, "Ancestor");
}

TEST(FindNearestCommonAncestorTest, SameSpecies) {
    TreeNode* root = buildTestTree();
    TreeNode* result = findNearestCommonAncestor(root, "Human", "Human");
    EXPECT_EQ(result->name, "Mammal");
}

TEST(FindNearestCommonAncestorTest, SpeciesNotInTree) {
    TreeNode* root = buildTestTree();
    TreeNode* result = findNearestCommonAncestor(root, "Human", "Dinosaur");
    EXPECT_EQ(result, nullptr);
}

TEST(FindNearestCommonAncestorTest, BothSpeciesNotInTree) {
    TreeNode* root = buildTestTree();
    TreeNode* result = findNearestCommonAncestor(root, "Dinosaur", "Alien");
    EXPECT_EQ(result, nullptr);
}

TEST(FindNearestCommonAncestorTest, SingleNodeTreeWithOneNonExistent) {
    TreeNode* root = new TreeNode("Root");
    TreeNode* result = findNearestCommonAncestor(root, "Root", "NonExistent");
    EXPECT_EQ(result, nullptr);
}

TEST(FindNearestCommonAncestorTest, SingleNodeTreeRootSelf) {
    TreeNode* root = new TreeNode("Root");
    TreeNode* result = findNearestCommonAncestor(root, "Root", "Root");
    EXPECT_EQ(result, nullptr);
}

TEST(FindNearestCommonAncestorTest, NullRoot) {
    TreeNode* result = findNearestCommonAncestor(nullptr, "Human", "Chimpanzee");
    EXPECT_EQ(result, nullptr);
}

// Main function
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}