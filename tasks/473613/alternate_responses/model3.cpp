#include <iostream>
#include <string>
#include <vector>

struct TreeNode {
    std::string name;
    std::vector<TreeNode*> children;

    TreeNode(std::string name) : name(name) {}
};

bool findPath(TreeNode* root, const std::string& species, std::vector<TreeNode*>& path) {
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

bool isAncestor(TreeNode* potentialAncestor, TreeNode* node, const std::vector<TreeNode*>& path) {
    auto it = std::find(path.begin(), path.end(), potentialAncestor);
    if (it == path.end()) return false;
    
    auto nodeIt = std::find(path.begin(), path.end(), node);
    if (nodeIt == path.end()) return false;
    
    return std::distance(path.begin(), it) < std::distance(path.begin(), nodeIt);
}

TreeNode* findNearestCommonAncestor(TreeNode* root, const std::string& species1, const std::string& species2) {
    if (!root) return nullptr;
    
    std::vector<TreeNode*> path1, path2;
    
    // Find paths to both species
    if (!findPath(root, species1, path1) || !findPath(root, species2, path2)) {
        return nullptr;
    }
    
    // Find the last common node in both paths
    TreeNode* lastCommon = nullptr;
    size_t i = 0;
    while (i < path1.size() && i < path2.size() && path1[i] == path2[i]) {
        lastCommon = path1[i];
        i++;
    }
    
    // Handle edge case where one species is ancestor of another
    if (lastCommon) {
        TreeNode* species1Node = path1.back();
        TreeNode* species2Node = path2.back();
        
        if (species1Node == lastCommon && isAncestor(species1Node, species2Node, path2)) {
            return i > 1 ? path1[i-2] : nullptr;
        }
        if (species2Node == lastCommon && isAncestor(species2Node, species1Node, path1)) {
            return i > 1 ? path2[i-2] : nullptr;
        }
    }
    
    return lastCommon;
}
