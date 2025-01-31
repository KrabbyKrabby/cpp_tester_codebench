#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <sstream>
#include <algorithm>

class FileSystem {
private:
    std::unordered_set<std::string> filePaths;

    bool isSubPath(const std::string& path1, const std::string& path2) const {
        return path1.length() < path2.length() && 
               path2.substr(0, path1.length()) == path1 &&
               path2[path1.length()] == '/';
    }

    std::string getParentPath(const std::string& path) const {
        size_t lastSlash = path.find_last_of('/');
        if (lastSlash == 0) return "/";
        return path.substr(0, lastSlash);
    }

public:
    std::string normalizePath(const std::string& path) {
        if (path.empty() || path[0] != '/') {
            throw std::invalid_argument("Path must start with '/'");
        }

        std::vector<std::string> components;
        std::stringstream ss(path);
        std::string component;

        // Split path by '/' and process components
        while (std::getline(ss, component, '/')) {
            if (component.empty() || component == ".") continue;
            if (component == "..") {
                if (!components.empty()) components.pop_back();
            } else {
                components.push_back(component);
            }
        }

        // Reconstruct normalized path
        if (components.empty()) return "/";
        
        std::string normalizedPath;
        for (const auto& comp : components) {
            normalizedPath += "/" + comp;
        }
        return normalizedPath;
    }

    void addFilePath(const std::string& path) {
        std::string normalizedFilePath = normalizePath(path);

        // Check for conflicts with existing paths
        for (const auto& existingPath : filePaths) {
            if (isSubPath(normalizedFilePath, existingPath) ||
                isSubPath(existingPath, normalizedFilePath)) {
                throw std::invalid_argument("Path conflicts with existing file or directory");
            }
        }

        filePaths.insert(normalizedFilePath);
    }

    std::vector<std::string> listFilesInDirectory(const std::string& directory) {
        std::string normalizedDir = normalizePath(directory);
        if (normalizedDir != "/" && normalizedDir.back() == '/') {
            normalizedDir.pop_back();
        }

        std::vector<std::string> files;
        for (const auto& path : filePaths) {
            std::string parentPath = getParentPath(path);
            if (parentPath == normalizedDir) {
                files.push_back(path);
            }
        }

        std::sort(files.begin(), files.end());
        return files;
    }

    bool filePathExists(const std::string& path) const {
        return filePaths.find(path) != filePaths.end();
    }
};