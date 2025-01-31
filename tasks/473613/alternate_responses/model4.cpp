#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct TreeNode {
    std::string name;
    std::vector<TreeNode*> children;

    TreeNode(std::string name) : name(name) {}
};

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

TreeNode* findNearestCommonAncestor(TreeNode* root, 
                                   const std::string& species1, 
                                   const std::string& species2) {
    if (!root) return nullptr;
    
    // Find paths from root to both species
    std::vector<TreeNode*> path1, path2;
    
    // If either species is not found, return nullptr
    if (!findPath(root, species1, path1) || !findPath(root, species2, path2)) {
        return nullptr;
    }
    
    // Handle the case where one species is ancestor of another
    if (path1.size() > 1 && path1[path1.size() - 2]->name == species2) {
        return path1.size() > 2 ? path1[path1.size() - 3] : nullptr;
    }
    if (path2.size() > 1 && path2[path2.size() - 2]->name == species1) {
        return path2.size() > 2 ? path2[path2.size() - 3] : nullptr;
    }
    
    // Find the nearest common ancestor
    size_t i = 0;
    TreeNode* lastCommon = nullptr;
    
    while (i < path1.size() && i < path2.size() && 
           path1[i] == path2[i]) {
        lastCommon = path1[i];
        i++;
    }
    
    // If the common ancestor is one of the species, return its ancestor
    if (lastCommon && (lastCommon->name == species1 || 
                      lastCommon->name == species2)) {
        return i > 1 ? path1[i - 2] : nullptr;
    }
    
    return lastCommon;
}