#include <iostream>
#include <string>
#include <vector>

struct TreeNode {
    std::string name;
    std::vector<TreeNode*> children;

    TreeNode(std::string name) : name(name) {}
};

bool findPath(TreeNode* root, const std::string& species, 
              std::vector<TreeNode*>& path) {
    if (!root) return false;
    
    path.push_back(root);
    
    if (root->name == species) {
        return true;
    }
    
    for (TreeNode* child : root->children) {
        if (findPath(child, species, path)) {
            return true;
        }
    }
    
    path.pop_back();
    return false;
}

TreeNode* findNearestCommonAncestor(TreeNode* root, 
                                   const std::string& species1, 
                                   const std::string& species2) {
    if (!root) return nullptr;
    
    // Find paths from root to both species
    std::vector<TreeNode*> path1, path2;
    bool found1 = findPath(root, species1, path1);
    bool found2 = findPath(root, species2, path2);
    
    // If either species is not found
    if (!found1 || !found2) {
        return nullptr;
    }
    
    // Handle case where one species is a direct ancestor of another
    if (species1 == root->name || species2 == root->name) {
        return nullptr;
    }
    
    // Find the last common node in both paths
    TreeNode* lastCommon = nullptr;
    size_t i = 0;
    while (i < path1.size() && i < path2.size() && 
           path1[i] == path2[i]) {
        lastCommon = path1[i];
        i++;
    }
    
    // If one species is direct ancestor of another
    if ((i == path1.size() && path1.back()->name == species1) || 
        (i == path2.size() && path2.back()->name == species2)) {
        // Return the ancestor of the ancestor species
        if (lastCommon == root) {
            return nullptr;
        }
        size_t j = 0;
        while (j < path1.size() && path1[j] != lastCommon) {
            j++;
        }
        if (j > 0) {
            return path1[j-1];
        }
        return nullptr;
    }
    
    return lastCommon;
}
