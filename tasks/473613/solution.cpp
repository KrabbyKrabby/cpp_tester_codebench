#include <iostream>
#include <string>
#include <vector>

struct TreeNode {
    std::string name;
    std::vector<TreeNode*> children;

    TreeNode(std::string name) : name(name) {}
};

// Helper function to find the path from the root to a species
bool findPath(TreeNode* root, const std::string& species, 
              std::vector<TreeNode*>& path) {
    if (!root) {
        return false;
    }

    // Add the current node to the
    path.push_back(root);
    
    // If current node is the target species
    if (root->name == species) {
        return true;
    }
    
    // Search in children
    for (TreeNode* child : root->children) {
        if (findPath(child, species, path)) {
            return true;
        }
    }
    
    // If species are not found in this path, remove the current node
    path.pop_back();
    return false;
}

// Function to check if one species is ancestor of another
bool isAncestor(TreeNode* potentialAncestor, TreeNode* node) {
    if (!node || !potentialAncestor) {
        return false;
    }
    if (node == potentialAncestor) {
        return true;
    }
    
    for (TreeNode* child : potentialAncestor->children) {
        if (isAncestor(child, node)) {
            return true;
        }
    }
    return false;
}

TreeNode* findNearestCommonAncestor(TreeNode* root, 
                                   const std::string& species1, 
                                   const std::string& species2) {
    if (!root){
        return nullptr;
    }
    
    // Find paths from the root to both species
    std::vector<TreeNode*> path1, path2;
    if (!findPath(root, species1, path1) || !findPath(root, species2, path2)) {
        return nullptr;  // One or both species not found
    }
    
    TreeNode* species1Node = path1.back();
    TreeNode* species2Node = path2.back();
    
    // If the exact same node, return its parent if it exists; otherwise nullptr
    if (species1Node == species2Node) {
        if (path1.size() > 1) {
            // Return the parent of this species
            return path1[path1.size() - 2];
        } else {
            // This species is the root, so no parent
            return nullptr;
        }
    }

    // Check if one species is ancestor of another
    if (isAncestor(species1Node, species2Node) || 
        isAncestor(species2Node, species1Node)) {
        // If one is ancestor of another, and it's the root, return nullptr
        if (path1.size() == 1 || path2.size() == 1) {
            return nullptr;
        }
        // Return the ancestor's ancestor
        return (path1.size() < path2.size()) ? 
               path1[path1.size()-2] : path2[path2.size()-2];
    }
    
    // Find the last common node in both paths
    TreeNode* lastCommon = nullptr;
    size_t minLength = std::min(path1.size(), path2.size());
    
    for (size_t i = 0; i < minLength; i++) {
        if (path1[i] == path2[i]) {
            lastCommon = path1[i];
        } else {
            break;
        }
    }
    
    return lastCommon;
}