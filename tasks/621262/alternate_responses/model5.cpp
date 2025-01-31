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

    bool isDirectoryPath(const std::string& path) const {
        return !path.empty() && path.back() == '/';
    }

    bool hasFileInDirectory(const std::string& dirPath) const {
        for (const auto& file : filePaths) {
            if (file.substr(0, dirPath.length()) == dirPath) {
                return true;
            }
        }
        return false;
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
            if (component.empty() || component == ".") {
                continue;
            }
            if (component == "..") {
                if (!components.empty()) {
                    components.pop_back();
                }
            } else {
                components.push_back(component);
            }
        }

        // Reconstruct normalized path
        std::string normalizedPath = "/";
        for (size_t i = 0; i < components.size(); ++i) {
            normalizedPath += components[i];
            if (i < components.size() - 1) {
                normalizedPath += "/";
            }
        }

        return normalizedPath;
    }

    void addFilePath(const std::string& path) {
        std::string normalizedPath = normalizePath(path);
        
        // Check if the normalized path is different from input
        if (normalizedPath != path) {
            throw std::invalid_argument("Path must be normalized before adding");
        }

        // Check if this is a directory path
        if (isDirectoryPath(normalizedPath)) {
            throw std::invalid_argument("Cannot add directory paths directly");
        }

        // Extract directory path
        size_t lastSlash = normalizedPath.find_last_of('/');
        std::string dirPath = normalizedPath.substr(0, lastSlash + 1);

        // Check for conflicts
        for (const auto& existingPath : filePaths) {
            // Check if new path conflicts with existing directory
            if (normalizedPath.substr(0, existingPath.length()) == existingPath) {
                throw std::invalid_argument("Path conflicts with existing file");
            }
            // Check if new path would create a file in a path that's already a file
            if (existingPath.substr(0, dirPath.length()) == dirPath) {
                throw std::invalid_argument("Cannot create file in path that conflicts with existing file");
            }
        }

        filePaths.insert(normalizedPath);
    }

    std::vector<std::string> listFilesInDirectory(const std::string& directory) {
        std::string normalizedDir = normalizePath(directory);
        if (!normalizedDir.empty() && normalizedDir.back() != '/') {
            normalizedDir += '/';
        }

        std::vector<std::string> files;
        for (const auto& file : filePaths) {
            if (file.substr(0, normalizedDir.length()) == normalizedDir) {
                // Check if this is a direct child (no additional slashes)
                std::string relativePath = file.substr(normalizedDir.length());
                if (relativePath.find('/') == std::string::npos) {
                    files.push_back(file);
                }
            }
        }

        std::sort(files.begin(), files.end());
        return files;
    }

    bool filePathExists(const std::string& path) const {
        return filePaths.find(path) != filePaths.end();
    }
};