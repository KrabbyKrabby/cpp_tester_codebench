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

        // Split path by '/'
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

        // Check if path exists as directory
        for (const auto& existingPath : filePaths) {
            if (isSubPath(normalizedFilePath, existingPath)) {
                throw std::invalid_argument("Cannot add file path that exists as directory");
            }
            if (isSubPath(existingPath, normalizedFilePath)) {
                throw std::invalid_argument("Cannot add file path that conflicts with existing file");
            }
        }

        filePaths.insert(normalizedFilePath);
    }

    bool filePathExists(const std::string& path) const {
        return filePaths.find(normalizePath(path)) != filePaths.end();
    }

    std::vector<std::string> listFilesInDirectory(const std::string& directory) {
        std::string normalizedDir = normalizePath(directory);
        if (normalizedDir != "/" && normalizedDir.back() != '/') {
            normalizedDir += "/";
        }

        std::vector<std::string> files;
        for (const auto& path : filePaths) {
            if (path == normalizedDir) continue;
            
            if (isSubPath(normalizedDir, path)) {
                // Check if it's a direct child (no additional slashes)
                std::string relativePath = path.substr(normalizedDir.length());
                if (relativePath.find('/') == std::string::npos) {
                    files.push_back(path);
                }
            }
        }

        std::sort(files.begin(), files.end());
        return files;
    }
};