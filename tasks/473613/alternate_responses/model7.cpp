#include <iostream>
#include <string>
#include <vector>

// Definition for a TreeNode in the phylogenetic tree
struct TreeNode {
    std::string name;   // Name of the species
    std::vector<TreeNode*> children; // Vector of pointers to child nodes

    TreeNode(std::string name) : name(name) {}
};

// Function to find the nearest common ancestor
TreeNode* findNearestCommonAncestor(TreeNode* root, const std::string& species1, const std::string& species2) {
    // Base case: If the tree is empty, return nullptr
    if (root == nullptr) {
        return nullptr;
    }

    // If the current node is one of the species, return the node
    if (root->name == species1 || root->name == species2) {
        return root;
    }

    // Recursively search for the species in the children
    int count = 0;
    TreeNode* ancestor = nullptr;
    for (TreeNode* child : root->children) {
        TreeNode* temp = findNearestCommonAncestor(child, species1, species2);
        if (temp != nullptr) {
            count++;
            ancestor = temp;
        }
    }

    // If both species are found in different subtrees, return the current node
    if (count == 2) {
        return root;
    }

    // If only one species is found, return the ancestor
    if (count == 1) {
        return ancestor;
    }

    // If neither species is found, return nullptr
    return nullptr;
}