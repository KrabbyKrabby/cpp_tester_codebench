#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

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

    // If the current node's name matches either of the species, return the current node
    if (root->name == species1 || root->name == species2) {
        return root;
    }

    // Recursively search for the species in the child nodes
    std::vector<TreeNode*> foundNodes;
    for (TreeNode* child : root->children) {
        TreeNode* found = findNearestCommonAncestor(child, species1, species2);
        if (found != nullptr) {
            foundNodes.push_back(found);
        }
    }

    // If both species are found in different child nodes, return the current node as the nearest common ancestor
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

// Function to find the path from the root to a given species
std::vector<TreeNode*> findPath(TreeNode* root, const std::string& species) {
    // Base case: If the tree is empty, return an empty path
    if (root == nullptr) {
        return {};
    }

    // If the current node's name matches the species, return a path containing the current node
    if (root->name == species) {
        return {root};
    }

    // Recursively search for the species in the child nodes
    for (TreeNode* child : root->children) {
        std::vector<TreeNode*> path = findPath(child, species);
        if (!path.empty()) {
            // If the species is found, add the current node to the path and return it
            path.insert(path.begin(), root);
            return path;
        }
    }

    // If the species is not found, return an empty path
    return {};
}

// Function to find the nearest common ancestor
TreeNode* findNearestCommonAncestor(TreeNode* root, const std::string& species1, const std::string& species2) {
    // Find the paths from the root to both species
    std::vector<TreeNode*> path1 = findPath(root, species1);
    std::vector<TreeNode*> path2 = findPath(root, species2);

    // If either species is not found, return nullptr
    if (path1.empty() || path2.empty()) {
        return nullptr;
    }

    // Find the nearest common ancestor by iterating through the paths in parallel
    TreeNode* nearestCommonAncestor = nullptr;
    for (size_t i = 0; i < std::min(path1.size(), path2.size()); ++i) {
        if (path1[i] == path2[i]) {
            nearestCommonAncestor = path1[i];
        } else {
            break;
        }
    }

    // If one of the species is a direct descendant of the other, return the ancestor of the other species
    if (nearestCommonAncestor == path1.back() || nearestCommonAncestor == path2.back()) {
        if (nearestCommonAncestor == root) {
            return nullptr;
        }
        // Find the parent of the nearest common ancestor
        for (TreeNode* child : root->children) {
            if (child == nearestCommonAncestor) {
                return root;
            }
            TreeNode* parent = findParent(child, nearestCommonAncestor);
            if (parent != nullptr) {
                return parent;
            }
        }
    }

    return nearestCommonAncestor;
}

// Function to find the parent of a given node
TreeNode* findParent(TreeNode* root, TreeNode* node) {
    // Base case: If the tree is empty, return nullptr
    if (root == nullptr) {
        return nullptr;
    }

    // If the current node is the parent of the given node, return the current node
    for (TreeNode* child : root->children) {
        if (child == node) {
            return root;
        }
    }

    // Recursively search for the parent in the child nodes
    for (TreeNode* child : root->children) {
        TreeNode* parent = findParent(child, node);
        if (parent != nullptr) {
            return parent;
        }
    }

    // If the parent is not found, return nullptr
    return nullptr;
}