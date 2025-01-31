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
    if (root == nullptr) {
        return nullptr;
    }

    // If the current node is one of the species, return the node
    if (root->name == species1 || root->name == species2) {
        return root;
    }

    // Recursively search for the species in the children of the current node
    std::vector<TreeNode*> foundNodes;
    for (TreeNode* child : root->children) {
        TreeNode* foundNode = findNearestCommonAncestor(child, species1, species2);
        if (foundNode != nullptr) {
            foundNodes.push_back(foundNode);
        }
    }

    // If both species are found in different subtrees, return the current node
    if (foundNodes.size() > 1) {
        return root;
    }

    // If only one species is found, return the found node
    if (foundNodes.size() == 1) {
        return foundNodes[0];
    }

    // If neither species is found, return nullptr
    return nullptr;
}
