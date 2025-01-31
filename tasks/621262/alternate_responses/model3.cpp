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

    // Helper function to check if one path is a subpath of another
    bool isSubPath(const std::string& path1, const std::string& path2) const {
        return (path1.length() < path2.length() &&
                path2.substr(0, path1.length()) == path1 &&
                path2[path1.length()] == '/');
    }

    // Helper function to get parent path
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
        std::istringstream iss(path);
        std::string component;
        
        // Split path by '/'
        size_t start = 1;
        while (start < path.length()) {
            size_t end = path.find('/', start);
            if (end == std::string::npos) end = path.length();
            std::string comp = path.substr(start, end - start);
            if (!comp.empty() && comp != ".") {
                if (comp == "..") {
                    if (!components.empty()) components.pop_back();
                } else {
                    components.push_back(comp);
                }
            }
            start = end + 1;
        }

        // Build normalized path
        if (components.empty()) return "/";
        
        std::string normalized = "";
        for (const auto& comp : components) {
            normalized += "/" + comp;
        }
        return normalized;
    }

    void addFilePath(const std::string& path) {
        std::string normalizedPath = normalizePath(path);

        // Check for conflicts with existing paths
        for (const auto& existingPath : filePaths) {
            if (isSubPath(normalizedPath, existingPath) ||
                isSubPath(existingPath, normalizedPath)) {
                throw std::invalid_argument("Path conflicts with existing file or directory");
            }
        }

        filePaths.insert(normalizedPath);
    }

    bool filePathExists(const std::string& path) const {
        try {
            std::string normalizedPath = normalizePath(path);
            return filePaths.find(normalizedPath) != filePaths.end();
        } catch (const std::invalid_argument&) {
            return false;
        }
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
};