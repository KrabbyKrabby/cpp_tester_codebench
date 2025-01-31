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
    // Base case: If the root is nullptr, return nullptr
    if (root == nullptr) {
        return nullptr;
    }

    // If the root's name matches either species, return the root
    if (root->name == species1 || root->name == species2) {
        return root;
    }

    // Initialize a vector to store the results of recursive calls
    std::vector<TreeNode*> results;

    // Recursively search for the species in the children of the root
    for (TreeNode* child : root->children) {
        TreeNode* result = findNearestCommonAncestor(child, species1, species2);
        if (result != nullptr) {
            results.push_back(result);
        }
    }

    // If both species are found in different subtrees, return the root
    if (results.size() > 1) {
        return root;
    }

    // If only one species is found, return the result
    if (results.size() == 1) {
        return results[0];
    }

    // If neither species is found, return nullptr
    return nullptr;
}