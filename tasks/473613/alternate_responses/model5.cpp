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
    
    // If species not found in this path, remove current node
    path.pop_back();
    return false;
}

TreeNode* findNearestCommonAncestor(TreeNode* root, const std::string& species1, 
                                   const std::string& species2) {
    if (!root) return nullptr;
    
    // Find paths from root to both species
    std::vector<TreeNode*> path1, path2;
    bool found1 = findPath(root, species1, path1);
    bool found2 = findPath(root, species2, path2);
    
    // If either species is not found, return nullptr
    if (!found1 || !found2) {
        return nullptr;
    }
    
    // Handle case where one species is an ancestor of another
    for (size_t i = 0; i < path1.size(); i++) {
        if (path1[i]->name == species1) {
            for (size_t j = 0; j <= i; j++) {
                if (path1[j]->name == species2) {
                    // species2 is ancestor of species1
                    return (j == 0) ? nullptr : path1[j-1];
                }
            }
        }
    }
    
    for (size_t i = 0; i < path2.size(); i++) {
        if (path2[i]->name == species2) {
            for (size_t j = 0; j <= i; j++) {
                if (path2[j]->name == species1) {
                    // species1 is ancestor of species2
                    return (j == 0) ? nullptr : path2[j-1];
                }
            }
        }
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
