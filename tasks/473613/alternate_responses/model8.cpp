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

    // If the current node is one of the species, return the current node
    if (root->name == species1 || root->name == species2) {
        return root;
    }

    // Recursively search for the species in the children
    TreeNode* ancestor1 = nullptr;
    TreeNode* ancestor2 = nullptr;
    for (TreeNode* child : root->children) {
        TreeNode* temp = findNearestCommonAncestor(child, species1, species2);
        if (temp != nullptr) {
            if (ancestor1 == nullptr) {
                ancestor1 = temp;
            } else {
                ancestor2 = temp;
                break; // Found both species, stop searching
            }
        }
    }

    // If both species are found in different subtrees, return the current node
    if (ancestor1 != nullptr && ancestor2 != nullptr) {
        return root;
    }

    // If only one species is found, return the ancestor of that species
    if (ancestor1 != nullptr) {
        return ancestor1;
    }

    // If neither species is found, return nullptr
    return nullptr;
}