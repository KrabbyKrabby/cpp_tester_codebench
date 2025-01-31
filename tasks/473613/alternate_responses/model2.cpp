#include <iostream>
#include <string>
#include <vector>

struct TreeNode {
    std::string name;
    std::vector<TreeNode*> children;

    TreeNode(std::string name) : name(name) {}
};

// Helper function to find path from root to target species
bool findPath(TreeNode* root, const std::string& species, 
              std::vector<TreeNode*>& path) {
    if (!root) return false;
    
    // Add current node to path
    path.push_back(root);
    
    // If current node is the target
    if (root->name == species) {
        return true;
    }
    
    // Check all children
    for (TreeNode* child : root->children) {
        if (findPath(child, species, path)) {
            return true;
        }
    }
    
    // If target not found in this path, remove current node
    path.pop_back();
    return false;
}

TreeNode* findNearestCommonAncestor(TreeNode* root, const std::string& species1, 
                                   const std::string& species2) {
    if (!root) return nullptr;
    
    // Find paths from root to both species
    std::vector<TreeNode*> path1, path2;
    
    // If either species is not found, return nullptr
    if (!findPath(root, species1, path1) || !findPath(root, species2, path2)) {
        return nullptr;
    }
    
    // Handle the case where one species is a direct ancestor of another
    if (species1 == path2[path2.size() - 2]->name || 
        species2 == path1[path1.size() - 2]->name) {
        // If the ancestor is the root, return nullptr
        if (path1.size() < 2 || path2.size() < 2) {
            return nullptr;
        }
        // Return the parent of the ancestor
        return (species1 == path2[path2.size() - 2]->name) ? 
               path2[path2.size() - 3] : path1[path1.size() - 3];
    }
    
    // Find the last common node in both paths
    TreeNode* lastCommon = nullptr;
    size_t i = 0;
    while (i < path1.size() && i < path2.size() && path1[i] == path2[i]) {
        lastCommon = path1[i];
        i++;
    }
    
    return lastCommon;
}